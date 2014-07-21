/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgInstanceGroup.h>
#include <pxgObject.h>
#include <pxgShader.h>
#include <pxgBuffer.h>

#include <algorithm>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/gl.h>

pxgInstanceGroup::pxgInstanceGroup(pxgObject *o): object(o)
{}


void pxgInstanceGroup::Update()
{
    using namespace glm;
    for(int i = 0;i<instances.size();i++)
    {
        instances[i]->transform = instances[i]->m_transform;
        instances[i]->transform = glm::translate(instances[i]->transform, instances[i]->pos);
        instances[i]->transform = glm::rotate(instances[i]->transform, instances[i]->rot.y, vec3(0,1,0));
        instances[i]->transform = glm::rotate(instances[i]->transform, instances[i]->rot.x, vec3(1,0,0));
        instances[i]->transform = glm::rotate(instances[i]->transform, instances[i]->rot.z, vec3(0,0,1));
        instances[i]->transform = glm::scale(instances[i]->transform, instances[i]->scaling);
    }
}

void pxgInstanceGroup::Render()
{
    if(!object)
        return;
    object->BeginRenderInstance();
    if(object->GetIBO()->GetSize())
    {
        if(object->GetFlags() & PXG_DRAW_WIREFRAME)
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

        for(int i = 0;i<instances.size();i++)
        {
            object->GetShader()->SetUniform(PXG_MODEL,glm::value_ptr(instances[i]->transform));
            glDrawElements(GL_TRIANGLES,object->GetIBO()->GetSize(),GL_UNSIGNED_INT,NULL);
        }

        if(object->GetFlags() & PXG_DRAW_WIREFRAME)
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

        object->EndRenderInstance();
        return;
    }
    if(object->GetFlags() & PXG_DRAW_WIREFRAME)
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    for(int i = 0;i<instances.size();i++)
    {
        object->GetShader()->SetUniform(PXG_MODEL,glm::value_ptr(instances[i]->transform));
        glDrawArrays(GL_TRIANGLES,0,object->GetVBO()->GetSize());
    }

    if(object->GetFlags() & PXG_DRAW_WIREFRAME)
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    object->EndRenderInstance();
}

int pxgInstanceGroup::AddInstance(pxgInstance* t)
{
    if(!t)
        return 0;
    instances.push_back(t);
    return instances.size();
}

int pxgInstanceGroup::GetInstanceCount()
{
    return instances.size();
}

void pxgInstanceGroup::RemoveInstance(int id)
{
    if(id<instances.size()&&id>0)
    {
        delete instances[id];
        instances.erase(instances.begin()+id);
    }
}

void pxgInstanceGroup::RemoveInstance(pxgInstance* inst)
{
    if(std::find(instances.begin(),instances.end(),inst)!=instances.end())
        instances.erase(std::find(instances.begin(),instances.end(),inst));
}
