/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgObject.h>

#include <pxgScene.h>


#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <pxgRigidBody.h>
#include <pxgBuffer.h>
#include <pxgShader.h>
#include <pxgTools.h>

#include <btBulletDynamicsCommon.h>


#include <algorithm>

using namespace glm;

pxgObject::pxgObject():shader(0), vbo(0), ibo(0), scene(NULL), pxgNode(), flags(0), rBody(NULL), mat(pxgMaterial()), visible(true)
{
    transformation = mat4(1.0f);
    scaling = vec3(1.0f);
    translation = rotation = vec3(0.0f);
}

pxgObject::~pxgObject()
{
    Destroy();
}

bool pxgObject::Init()
{
    PXG::glGenVertexArrays(1,&vao);
    if(vao==0)
    {
        PXG::Log("pxgObject::Init(): error creating VAO",ERR);
        return false;
    }
    PXG::Log("pxgObject::Init(): created VAO");
    InitVBO();
    InitIBO();
	return true;
}

bool pxgObject::InitVBO()
{
    if(vao==0)
        return false;
    PXG::glBindVertexArray(vao);
    vbo = new pxgBuffer;
    vbo->Init();
    PXG::glBindBuffer(GL_ARRAY_BUFFER,vbo->GetName());
    PXG::glBindVertexArray(NULL);
    return true;
}

bool pxgObject::InitIBO()
{
    if(vao==0)
        return false;
    PXG::glBindVertexArray(vao);
    ibo = new pxgBuffer;
    ibo->Init();
    PXG::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo->GetName());
    PXG::glBindVertexArray(NULL);
}

void pxgObject::SetShader(pxgShader* _shader)
{
  shader = _shader;
  for(int i = 0;i<meshList.size();i++)
  {
    if(meshList[i]!=NULL)
        meshList[i]->SetShader(_shader);
  }
}
void pxgObject::SetScene(pxgScene* pScene)
{
    scene = pScene;
    if(meshList.size()<=0)
        return;
    for(int i = 0;i<meshList.size();i++)
    {
        if(meshList[i]!=NULL)
            meshList[i]->SetScene(scene);
    }
}

void pxgObject::SetWorld(pxgWorld* pWorld)
{
    world = pWorld;
    if(meshList.size()<=0)
        return;
    for(int i = 0;i<meshList.size();i++)
    {
        if(meshList[i]!=NULL)
            meshList[i]->SetWorld(world);
    }
}


void pxgObject::SetVertexLayout(PXG_VERTEX_LAYOUT l)
{
    layout = l;
    if(vao==0)
    {
        PXG::Log("pxgObject::SetVertexLayout() error: please call Init() first",WARN);
        return;
    }

    PXG::glBindVertexArray(vao);
    switch(layout)
    {
        case PXG_VERTEX2D:
        {
            PXG::Log("pxgObject::SetVertexLayout(): creating attrib pointer for attribute: PXG_POSITION");
            PXG::glVertexAttribPointer(PXG_POSITION, sizeof(glm::vec2)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex2d), (void*)offsetof(pxgVertex2d,pos));
            PXG::glEnableVertexAttribArray(PXG_POSITION);
            PXG::Log("pxgObject::SetVertexLayout(): creating attrib pointer for attribute: PXG_TEXCOORD");
            PXG::glVertexAttribPointer(PXG_TEXCOORD,sizeof(glm::vec3)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex2d), (void*)offsetof(pxgVertex2d, color));
            PXG::glEnableVertexAttribArray(PXG_TEXCOORD);
            break;
        }
        case PXG_VERTEX2DT:
        {
            PXG::Log("pxgObject::SetVertexLayout(): creating attrib pointer for attribute: PXG_POSITION");
            PXG::glVertexAttribPointer(PXG_POSITION, sizeof(glm::vec2)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex2dT), (void*)offsetof(pxgVertex2dT,pos));
            PXG::glEnableVertexAttribArray(PXG_POSITION);
            PXG::Log("pxgObject::SetVertexLayout(): creating attrib pointer for attribute: PXG_TEXCOORD");
            PXG::glVertexAttribPointer(PXG_TEXCOORD,sizeof(glm::vec2)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex2dT), (void*)offsetof(pxgVertex2dT, tc));
            PXG::glEnableVertexAttribArray(PXG_TEXCOORD);
            break;
        }
        case PXG_VERTEX3D:
        {
            PXG::Log("pxgObject::SetVertexLayout(): creating attrib pointer for attribute: PXG_POSITION");
            PXG::glVertexAttribPointer(PXG_POSITION, sizeof(glm::vec3)/4, GL_FLOAT, GL_FALSE, 0, (void*)offsetof(pxgVertex3d,pos));
            PXG::glEnableVertexAttribArray(PXG_POSITION);
            break;
        }
        case PXG_VERTEX3DT:
        {
            PXG::Log("pxgObject::SetVertexLayout(): creating attrib pointer for attribute: PXG_POSITION");
            PXG::glVertexAttribPointer(PXG_POSITION, sizeof(glm::vec3)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex3dT), (void*)offsetof(pxgVertex3dT,pos));
            PXG::glEnableVertexAttribArray(PXG_POSITION);
            PXG::Log("pxgObject::SetVertexLayout(): creating attrib pointer for attribute: PXG_TEXCOORD");
            PXG::glVertexAttribPointer(PXG_TEXCOORD,sizeof(glm::vec2)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex3dT), (void*)offsetof(pxgVertex3dT, tc));
            PXG::glEnableVertexAttribArray(PXG_TEXCOORD);
            break;
        }
        case PXG_VERTEX3DTN:
        {
            PXG::Log("pxgObject::SetVertexLayout(): creating attrib pointer for attribute: PXG_POSITION");
            PXG::glVertexAttribPointer(PXG_POSITION, sizeof(glm::vec3)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex3dTN), (void*)offsetof(pxgVertex3dTN,pos));
            PXG::glEnableVertexAttribArray(PXG_POSITION);
            PXG::Log("pxgObject::SetVertexLayout(): creating attrib pointer for attribute: PXG_TEXCOORD");
            PXG::glVertexAttribPointer(PXG_TEXCOORD,sizeof(glm::vec2)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex3dTN), (void*)offsetof(pxgVertex3dTN, tc));
            PXG::glEnableVertexAttribArray(PXG_TEXCOORD);
            PXG::Log("pxgObject::SetVertexLayout(): creating attrib pointer for attribute: PXG_NORMAL");
            PXG::glVertexAttribPointer(PXG_NORMAL,sizeof(glm::vec3)/4, GL_FLOAT, GL_TRUE, sizeof(pxgVertex3dTN), (void*)offsetof(pxgVertex3dTN, normal));
            PXG::glEnableVertexAttribArray(PXG_NORMAL);
            break;
        }
        case PXG_VERTEX3DN:
        {
            PXG::Log("pxgObject::SetVertexLayout(): creating attrib pointer for attribute: PXG_POSITION");
            PXG::glVertexAttribPointer(PXG_POSITION, sizeof(glm::vec3)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex3dN), (void*)offsetof(pxgVertex3dN,pos));
            PXG::glEnableVertexAttribArray(PXG_POSITION);
            PXG::Log("pxgObject::SetVertexLayout(): creating attrib pointer for attribute: PXG_NORMAL");
            PXG::glVertexAttribPointer(PXG_NORMAL,sizeof(glm::vec3)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex3dN), (void*)offsetof(pxgVertex3dN, normal));
            PXG::glEnableVertexAttribArray(PXG_NORMAL);
            break;
        }
        case PXG_VERTEX3DTANIM:
        {
            PXG::Log("pxgObject::SetVertexLayout(): creating attrib pointer for attribute: PXG_POSITION");
            PXG::glVertexAttribPointer(PXG_POSITION, sizeof(glm::vec3)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex3dTAnim), (void*)offsetof(pxgVertex3dTAnim,pos));
            PXG::glEnableVertexAttribArray(PXG_POSITION);
            PXG::Log("pxgObject::SetVertexLayout(): creating attrib pointer for attribute: PXG_TEXCOORD");
            PXG::glVertexAttribPointer(PXG_TEXCOORD,sizeof(glm::vec2)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex3dTAnim), (void*)offsetof(pxgVertex3dTAnim, tc));
            PXG::glEnableVertexAttribArray(PXG_TEXCOORD);
            PXG::Log("pxgObject::SetVertexLayout(): creating attrib pointer for attribute: PXG_BONE_INDEX");
            PXG::glVertexAttribIPointer(PXG_BONE_INDEX,sizeof(glm::ivec4)/4, GL_INT, sizeof(pxgVertex3dTAnim), (void*)offsetof(pxgVertex3dTAnim, indices));
            PXG::glEnableVertexAttribArray(PXG_BONE_INDEX);
            PXG::Log("pxgObject::SetVertexLayout(): creating attrib pointer for attribute: PXG_BONE_WEIGHT");
            PXG::glVertexAttribPointer(PXG_BONE_WEIGHT,sizeof(glm::vec4)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex3dTAnim), (void*)offsetof(pxgVertex3dTAnim, weights));
            PXG::glEnableVertexAttribArray(PXG_BONE_WEIGHT);
            break;
        }

    }
    PXG::glBindVertexArray(NULL);
}

void pxgObject::SetVertices(void* data, int sz, int count)
{
    if(vbo==0)
        return;
    //PXG::glBindVertexArray(vao);
    vbo->SetData(VERTEX,data,sz,count);
}

void pxgObject::SetIndices(void* data, int sz, int count)
{
    if(ibo==0)
        return;
    PXG::glBindVertexArray(vao);
    ibo->SetData(ELEMENT,data,sz,count);
}

void pxgObject::SetFlags(int flag)
{
    flags = flag;
    for(int i = 0;i<meshList.size();i++)
    {
        meshList[i]->SetFlags(flags);
    }
}

void pxgObject::SetTexture(pxgTexture *tex, PXG_TEXTURE_STAGE stage)
{
    if(tex == NULL)
    {
        textures.erase(stage);
        return;
    }

    textures[stage] = tex;
    return;
}

void pxgObject::Update()
{
    if(flags & PXG_ENABLE_DYNAMICS)
    {
        UpdateFromRigidBody();
    }
    else
    {
        UpdateTransformation();
        UpdateChildMeshes();
    }
    return;
}

bool pxgObject::Render()
{
    RenderChildMeshes();
    if(flags & PXG_ENABLE_PICKING)
        return DrawPicking();
    if(scene==NULL)
    {
        PXG::Log("scene err");
        return false;
    }
    if(shader!=NULL)
    {
        if(!shader->Use())
        {
            PXG::Log("shader err");
            return false;
        }
    }
    else
    {
        PXG::Log("shader err 1");
        return false;
    }
    int t = 0;
    if(flags & PXG_DRAW_BILLBOARD)
        return DrawBillboard();
    SetupMaterial();
    int ut;
    if(textures.size()>0)
        ut = 1;
    else
        ut = 0;
    shader->SetUniform(PXG_USE_TEXTURE,&ut);
    for(texObj::iterator i = textures.begin();i!=textures.end();i++)
    {
        if(i->second)
            shader->SetTexture((PXG_TEXTURE_STAGE)(i->first),i->second);
    }

    shader->SetUniform(PXG_MODEL,value_ptr(transformation));
    shader->SetUniform(PXG_VIEW_PROJECTION, value_ptr(scene->GetVP()));
    shader->SetUniform(PXG_VIEW,value_ptr(scene->GetView()));
    shader->SetUniform(PXG_PROJECTION,value_ptr(scene->GetProj()));
    if(flags & PXG_USE_LIGHTING)
        {
            std::vector<pxgLight*> l = scene->GetLights(this);
            if(l.size()>0)
            {
                for(int i = 0;i<l.size();i++)
                    shader->SetLight(i,l[i]);
            }
        }
    else
        shader->SetLightCount(0);
    PXG::glBindVertexArray(vao);
        if(ibo->GetSize())
        {
            if(flags & PXG_DRAW_WIREFRAME)
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

                glDrawElements(GL_TRIANGLES,ibo->GetSize(),GL_UNSIGNED_INT,NULL);

            if(flags & PXG_DRAW_WIREFRAME)
                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

            shader->Unuse();
            for(texObj::iterator i = textures.begin();i!=textures.end();i++)
            {
                if(i->second)
                    i->second->Unuse();
            }
            PXG::glBindVertexArray(0);
            return true;
        }
    if(flags & PXG_DRAW_WIREFRAME)
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

        glDrawArrays(GL_TRIANGLES,0,vbo->GetSize());

    if(flags & PXG_DRAW_WIREFRAME)
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    shader->Unuse();
    for(texObj::iterator i = textures.begin();i!=textures.end();i++)
    {
        if(i->second)
            i->second->Unuse();
    }
    PXG::glBindVertexArray(0);
    return true;
}

void pxgObject::AddChildMesh(pxgObject* obj)
{
    //if(obj==NULL)
    //    return;
    meshList.push_back(obj);
}

void pxgObject::RemoveChildsMesh(pxgObject* obj)
{
    if(obj==NULL)
        return;
    auto i = find(meshList.begin(),meshList.end(),obj);
    if(i!=meshList.end())
        meshList.erase(i);
    return;
}

void pxgObject::UpdateChildMeshes()
{
    if(meshList.size()<=0)
        return;
    for(MeshList::iterator i = meshList.begin();i!=meshList.end();i++)
    {
        pxgObject* obj = *i;
        obj->Transform(transformation);
        obj->Update();
    }
    return;
}

void pxgObject::RenderChildMeshes()
{
    if(meshList.size()<=0)
        return;
    for(MeshList::iterator i = meshList.begin();i!=meshList.end();i++)
    {
        pxgObject* obj = *i;
        obj->Render();
    }
    return;
}

void pxgObject::SetupMaterial()
{
    if(shader==NULL||scene==NULL)
        return;
    shader->SetUniform(PXG_DIFFUSE,value_ptr(mat.Diffuse()));
    shader->SetUniform(PXG_AMBIENT,value_ptr(mat.Ambient()));
    shader->SetUniform(PXG_SPECULAR,value_ptr(mat.Specular()));
    float s = mat.Shininess();
    shader->SetUniform(PXG_SHININESS,&s);
    shader->SetUniform(PXG_SCENE_AMBIENT,value_ptr(scene->GetAmbient()));
    return;
}

bool pxgObject::InitBillboard(float width, float height)
{
    PXG::glGenVertexArrays(1,&bbVAO);
    if(bbVAO==0)
    {
        PXG::Log("pxgObject::InitBillboard(): error creating VAO",ERR);
        return false;
    }
    bbSize = vec2(1);
    PXG::glBindVertexArray(bbVAO);
    bbVB = new pxgBuffer;
    bbVB->Init();
    PXG::glBindBuffer(GL_ARRAY_BUFFER,bbVB->GetName());
    //PXG::glBindVertexArray(NULL);
    bbIB = new pxgBuffer;
    bbIB->Init();
    PXG::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,bbIB->GetName());

    pxgVertex3dT data[] =
    {
        { vec3(-width/2.f, height/2.f,0),vec2(0.f,0.f) },
        { vec3(width/2.f, height/2.f,0),vec2(1.0f,0.f) },
        { vec3(width/2.f, -height/2.f,0),vec2(1.0f,1.0f) },
        { vec3(-width/2.f,-height/2.f,0),vec2(0.f,1.0f) }
    };

    GLuint elements[] =
    {
        0, 3, 1,
        3, 2, 1
    };

    bbVB->SetData(VERTEX,(void*)data,sizeof(data),sizeof(data)/sizeof(pxgVertex3dT));
    bbIB->SetData(ELEMENT, (void*)elements, sizeof(elements),sizeof(elements)/sizeof(GLuint));

    PXG::glVertexAttribPointer(PXG_POSITION, sizeof(glm::vec3)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex3dT), (void*)offsetof(pxgVertex3dT,pos));
    PXG::glEnableVertexAttribArray(PXG_POSITION);
    PXG::glVertexAttribPointer(PXG_TEXCOORD,sizeof(glm::vec2)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex3dT), (void*)offsetof(pxgVertex3dT, tc));
    PXG::glEnableVertexAttribArray(PXG_TEXCOORD);
    return true;
}

void pxgObject::BillboardTexture(pxgTexture* tex)
{
    bbTex = tex;
    return;
}

void pxgObject::BillboardColor(glm::vec4 color)
{
    bbColor = color;
    return;
}

bool pxgObject::DrawBillboard()
{
    mat3 rotMat(scene->GetView());
    glm::mat4 bbMat = m_transform;
    bbMat *= glm::mat4(glm::inverse(rotMat));
    //bbMat = glm::transform(bbMat,m_transform);
    bbMat = glm::scale(bbMat,scaling);
    shader->SetUniform(PXG_MODEL,value_ptr(bbMat));
    shader->SetUniform(PXG_VIEW_PROJECTION, value_ptr(scene->GetVP()));
    shader->SetUniform(PXG_VIEW,value_ptr(scene->GetView()));
    shader->SetUniform(PXG_PROJECTION,value_ptr(scene->GetProj()));
    shader->SetLightCount(0);

    int ut;
    if(bbTex==NULL)
        ut = 0;
    else
        ut = 1;
    shader->SetUniform(PXG_USE_TEXTURE,&ut);
    if(bbVAO==0)
        return false;
    PXG::glBindVertexArray(bbVAO);
    if(bbVB==NULL)
        return false;
    if(bbIB==NULL)
        return false;
    if(ut)
        shader->SetTexture(PXG_TEXTURE0,bbTex);
    else
        shader->SetUniform(PXG_DIFFUSE, glm::value_ptr(glm::vec3(bbColor)));
    if(!bbVB->Use()) return false;
    if(!bbIB->Use()) return false;
    if(flags & PXG_DRAW_WIREFRAME)
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    glDrawElements(GL_TRIANGLES,bbIB->GetSize(),GL_UNSIGNED_INT,NULL);

    if(flags & PXG_DRAW_WIREFRAME)
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    return true;
}

void pxgObject::UpdateFromRigidBody()
{
    if(!rBody)
        return;
    btVector3 btT = rBody->GetPosition();
    glm::vec3 t(btT.getX(),btT.getY(), btT.getZ());
    translation = t;
    btQuaternion btR = rBody->GetRotation();
    glm::quat r(btR.getW(), btR.getX(), btR.getY(), btR.getZ());
    glm::mat4 trans = translate(glm::mat4(1),t);
    glm::mat4 rot = toMat4(r);
    transformation = trans*rot;
    transformation = scale(transformation,scaling);
}

bool pxgObject::DrawPicking()
{
    if(scene==NULL)
    {
        PXG::Log("scene err");
        return false;
    }

    shader->SetUniform(PXG_MODEL,value_ptr(transformation));
    shader->SetUniform(PXG_VIEW_PROJECTION, value_ptr(scene->GetVP()));
    shader->SetUniform(PXG_VIEW,value_ptr(scene->GetView()));
    shader->SetUniform(PXG_PROJECTION,value_ptr(scene->GetProj()));

    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D,0);

    PXG::glBindVertexArray(vao);
        if(ibo->GetSize())
        {
            glDrawElements(GL_TRIANGLES,ibo->GetSize(),GL_UNSIGNED_INT,NULL);

            PXG::glBindVertexArray(0);
            return true;
        }

        glDrawArrays(GL_TRIANGLES,0,vbo->GetSize());
    PXG::glBindVertexArray(0);
    return true;

}

void pxgObject::BeginRenderInstance()
{
    if(scene==NULL)
    {
        PXG::Log("scene err");
        return;
    }
    if(shader!=NULL)
    {
        if(!shader->Use())
        {
            PXG::Log("shader err");
            return;
        }
    }
    else
    {
        PXG::Log("shader err 1");
        return;
    }
    int t = 0;
    SetupMaterial();
    int ut;
    if(textures.size()>0)
        ut = 1;
    else
        ut = 0;
    shader->SetUniform(PXG_USE_TEXTURE,&ut);
    shader->SetUniform(PXG_VIEW_PROJECTION, value_ptr(scene->GetVP()));
    shader->SetUniform(PXG_VIEW,value_ptr(scene->GetView()));
    shader->SetUniform(PXG_PROJECTION,value_ptr(scene->GetProj()));

    for(texObj::iterator i = textures.begin();i!=textures.end();i++)
    {
        if(i->second)
            shader->SetTexture((PXG_TEXTURE_STAGE)(i->first),i->second);
    }

    if(flags & PXG_USE_LIGHTING)
        {
            std::vector<pxgLight*> l = scene->GetLights(this);
            if(l.size()>0)
            {
                for(int i = 0;i<l.size();i++)
                    shader->SetLight(i,l[i]);
            }
        }
    else
        shader->SetLightCount(0);
    PXG::glBindVertexArray(vao);
    return;
}

void pxgObject::EndRenderInstance()
{
    if(!shader)
        return;
    shader->Unuse();
    for(texObj::iterator i = textures.begin();i!=textures.end();i++)
    {
        if(i->second)
            i->second->Unuse();
    }
    PXG::glBindVertexArray(0);
}

void pxgObject::Destroy()
{
    if(scene)
        scene->RemoveObject(this);
    if(vbo)
        delete vbo;
    if(ibo)
        delete ibo;
    if(vao)
        PXG::glDeleteVertexArrays(1,&vao);
    for(int i = 0;i<meshList.size();i++)
    {
        meshList[i]->Destroy();
        delete meshList[i];
    }
}
