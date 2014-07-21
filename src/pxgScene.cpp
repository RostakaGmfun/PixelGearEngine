/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgScene.h>
#include <pxgTools.h>
#include <pxgObject.h>
#include <pxgLight.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <pxgCamera.h>
#include <pxgWorld.h>
#include <pxgUniforms.h>
#include <pxgShader.h>
#include <pxgInstanceGroup.h>


using namespace glm;

pxgScene::pxgScene(): camera(NULL), pxgNode(), world(NULL)
{
}

bool pxgScene::Init()
{
    return true;
}

void pxgScene::AddObject(pxgObject *obj)
{
    if(obj!=NULL)
    {
        objects.push_back(obj);
        obj->SetScene(this);
        obj->SetWorld(world);
    }
    return;
}

void pxgScene::RemoveObject(pxgObject* obj)
{
    auto it = std::find(objects.begin(),objects.end(),obj);
    if(it!=objects.end())
        objects.erase(it);
}

bool pxgScene::Render()
{
    for(int i = 0;i<objects.size();i++)
    {
        if(objects[i]->IsVisible())
            objects[i]->Render();
    }
    DrawInstances();

     for(int i = 0;i<instanceGroups.size();i++)
    {
        instanceGroups[i]->Render();
    }
    return true;
}

void pxgScene::Update()
{

    UpdateTransformation();
    if(camera!=NULL)
        camera->Update();

    for(int i = 0;i<objects.size();i++)
    {
        objects[i]->Update();
        objects[i]->Transform(transformation);
    }

     for(int i = 0;i<instanceGroups.size();i++)
    {
        instanceGroups[i]->Update();
    }
}

void pxgScene::Destroy()
{
}

const glm::mat4 pxgScene::GetView() const
{
 if(camera==NULL)
 {
    PXG::Log("pxgScene warning - camera not set", WARN);
    return glm::mat4(1);
 }
 return camera->GetView();
}

const glm::mat4 pxgScene::GetProj() const
{
 if(camera==NULL)
 {
    PXG::Log("pxgScene warning - camera not set", WARN);
    return glm::mat4(1);
 }
 return camera->GetProj();
}

const glm::mat4 pxgScene::GetVP() const
{
 if(camera==NULL)
 {
    PXG::Log("pxgScene warning - camera not set", WARN);
    return glm::mat4(1);
 }
 return camera->GetVP();
}

void pxgScene::AddLight(pxgLight* light)
{
    if(light==NULL)
        return;
    lights.push_back(new __pxg_light(light));
    light->SetParent(this);
    return;
}

void pxgScene::RemoveLight(pxgLight* light)
{
    if(light == NULL)
        return;
    for(std::vector<__pxg_light*>::iterator i = lights.begin();i!=lights.end();i++)
    {
        __pxg_light* l = *i;
        if(l->l == light)
        {
            l->l->SetParent(NULL);
            lights.erase(i);
            return;
        }
    }
}

void pxgScene::EnableLight(pxgLight* light)
{
    if(light == NULL)
        return;
    for(std::vector<__pxg_light*>::iterator i = lights.begin();i!=lights.end();i++)
    {
        __pxg_light* l = *i;
        if(l->l == light)
        {
            l->enabled = true;
            return;
        }
    }
    return;
}

void pxgScene::DisableLight(pxgLight* light)
{
    if(light == NULL)
        return;
    for(std::vector<__pxg_light*>::iterator i = lights.begin();i!=lights.end();i++)
    {
        __pxg_light* l = *i;
        if(l->l == light)
        {
            l->enabled = false;
            return;
        }
    }
    return;
}

size_t pxgScene::GetLightCount(pxgObject *obj) const
{
    if(obj == NULL)
        return 0;
    if(std::find(objects.begin(),objects.end(),obj)==objects.end())
        return 0;
    size_t c = 0;
    vec3 p = obj->GetTranslation();
    for(int i = 0;i!=lights.size();i++)
    {
        // the square distance local to the scene transformation is used here to avoid square root computation and additional vector transformation
        if(glm::distance2(lights[i]->l->GetTranslation(),p)<=lights[i]->l->GetAttenuation()*lights[i]->l->GetAttenuation()||lights[i]->l->GetType()==PXG_LIGHT_DIRECTIONAL)
            c++;
    }
}


std::vector<pxgLight*> pxgScene::GetLights(pxgObject* obj) const
{
    std::vector<pxgLight*> ls;
    if(obj == NULL)
        return ls;
    //if(std::find(objects.begin(),objects.end(),obj)==objects.end())
    //    return ls;
    if(lights.size()>0)
        for(int i = 0;i<lights.size();i++)
        {
            if(lights[i]!=NULL)
            {
                if(lights[i]->enabled)
                    ls.push_back(lights[i]->l);
            }
        }
    return ls;
}

void pxgScene::AddInstance(pxgObject* obj, glm::mat4 mat)
{
    if(obj==NULL)
        return;
    if(instances.find(obj)==instances.end())
    {
        std::vector<mat4> m;
        m.push_back(mat);
        instances[obj] = m;
    }
    else
    {
        instances[obj].push_back(mat);
    }
    obj->SetScene(this);
}

void pxgScene::DrawInstances()
{
    for(auto i = instances.begin();i!=instances.end();i++)
    {
        if(i->first!=NULL)
            for(int j = 0;j<i->second.size();j++)
            {
                i->first->Transform(i->second[j]);
                i->first->Render();
            }
    }
}

void pxgScene::DrawForPicking()
{
    if(objects.size()<=0||!world)
        return;
    if(!world->GetPickShader())
        return;
    pxgShader *ps = world->GetPickShader();
    ps->Use();
    for(int i = 0;i<objects.size();i++)
    {
        objects[i]->SetFlags(objects[i]->GetFlags() | PXG_ENABLE_PICKING);
        pxgShader* prev = objects[i]->GetShader();
        objects[i]->SetShader(world->GetPickShader());
        int id = i+1;
        int r = (id & 0x000000FF) >>  0;
        int g = (id & 0x0000FF00) >>  8;
        int b = (id & 0x00FF0000) >> 16;
        glm::vec3 pickC = glm::vec3(r/255.0f,g/255.0f,b/255.0f);
        ps->SetUniform(PXG_PICK_COLOR,glm::value_ptr(pickC));
        //if(objects[i]->IsVisible())
            objects[i]->Render();
        objects[i]->SetShader(prev);
        objects[i]->SetFlags(objects[i]->GetFlags() & ~PXG_ENABLE_PICKING);
    }
}

pxgObject* pxgScene::GetObjectByID(int id)
{
    if(id<0||id>objects.size())
        return NULL;
    return objects[id];

}

void pxgScene::AddInstanceGroup(pxgInstanceGroup* i)
{
    if(i)
        instanceGroups.push_back(i);
}
