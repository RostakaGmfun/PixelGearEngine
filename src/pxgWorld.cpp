/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgWorld.h>

#include <pxgObject.h>
#include <pxgMeshLib.h>
#include <pxgCamera.h>
#include <pxgScene.h>
#include <pxgShaderLib.h>
#include <pxgShader.h>
#include <pxgRigidBody.h>

#include <algorithm>

#include <pxgBuffer.h>

#include <glm/gtc/type_ptr.hpp>

#include <../SOIL.h>
#include <pxgTools.h>


pxgWorld::pxgWorld(): skybox(NULL), cam(NULL), root(NULL), terrain(NULL), pw(NULL), postRenderTarget(NULL),
    postShader(NULL), frameQuad(NULL), postFramebuffer(0), postRenderbuffer(0), frameVAO(0), postEnabled(false),
    pickBuffer(NULL), pickFramebuffer(0), pickRenderbuffer(0), pickFrameQuad(NULL), pickScene(NULL), pickVAO(0),
    pickingEnabled(false), debugPicking(false)
{
}

void pxgWorld::AddScene(pxgScene* scene)
{
    if(scene==NULL)
        return;
    scenes.push_back(scene);
    scene->SetWorld(this);
    scene->SetCamera(cam);
    return;
}

void pxgWorld::RemoveScene(pxgScene *scene)
{
    if(!scene)
        return;
    auto it = std::find(scenes.begin(),scenes.end(),scene);
    if(it!=scenes.end() )
            scenes.erase(it);
    return;
}

void pxgWorld::Update()
{
    if(pw)
        pw->Update(1/60.f);
    for(int i = 0; i<scenes.size();i++)
        scenes[i]->Update();
    if(root==NULL||cam==NULL)
        return;
    root->Translate(cam->GetTranslation());
    root->Update();
    cloudShader->SetUniform("move",glm::value_ptr(glm::vec2(cloudsMove,cloudsMove)));
    cloudsMove+=0.00005f;
    if(terrain) terrain->Update();
    return;
}

bool pxgWorld::Render()
{
    if(postEnabled&&postFramebuffer&&postRenderbuffer)
    {
        const GLenum draw_buffer = GL_COLOR_ATTACHMENT0;
        PXG::glDrawBuffers (1, &draw_buffer);

        PXG::glBindFramebuffer(GL_FRAMEBUFFER,postFramebuffer);
        glViewport(0,0,postW,postH);
    }
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    for(int i = 0;i<scenes.size();i++)
        scenes[i]->Render();
    if(terrain) terrain->Render();
    if(root!=NULL)
    {
        glCullFace(GL_FRONT);
        GLint oldDepthFunc;
        glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);
        glDepthFunc(GL_LEQUAL);
        root->Render();
        glCullFace(GL_BACK);
        glDepthFunc(oldDepthFunc);
    }
    if(postEnabled)
    {
        PXG::glBindFramebuffer(GL_FRAMEBUFFER,0);
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        if(frameQuad&&postRenderTarget&&frameVAO&&postShader)
        {
            glDisable(GL_DEPTH_TEST);
            glCullFace(GL_FRONT);
            PXG::glBindVertexArray(frameVAO);
            glActiveTexture(GL_TEXTURE0);
            postRenderTarget->Use(0);
            postShader->Use();
            glDrawArrays(GL_TRIANGLES,0,6);
            glCullFace(GL_BACK);
            glEnable(GL_DEPTH_TEST);
        }
    }
    return true;
}

bool pxgWorld::InitSkybox()
{
    if(cam==NULL||skybox!=NULL)
        return false;
    pxgMeshLib* ml = new pxgMeshLib();
    skybox = ml->LoadFromFile("blends/sphere.obj", PXG_WAVEFRONT_OBJ);
    if(skybox==NULL)
        return false;
    skybox->Scale(glm::vec3(cam->GetFar()));
    skybox->Translate(glm::vec3(cam->GetTranslation()));
    skybox->Update();
    pxgShaderLib* sl = new pxgShaderLib;
    pxgShader *sk = sl->InitSkyboxShader();
    if(sk==NULL)
        return false;
    //cloudShader = sl->InitCloudsShader();
    //cloudsMove = 0;
    //if(cloudShader==NULL)
   //     return false;
    skybox->SetShader(sk);
    //pxgObject* clouds = ml->CreatePlaneMesh(1,1);
   // if(clouds==NULL)
    //    return false;
    //clouds->Scale(glm::vec3(10000));
   // clouds->Translate(glm::vec3(0,cam->GetTranslation().z+700,0));
    //clouds->Rotate(glm::vec3(0,0,0));
   // clouds->Update();
    pxgTexture* tex = new pxgTexture();
    tex->LoadTexture("blends/noise1.png",PXG_FILTER_LINEAR|PXG_TEX_REPEAT_X|PXG_TEX_REPEAT_Y);
   // clouds->SetShader(cloudShader);
   // clouds->SetTexture(tex,PXG_TEXTURE0);
    root = new pxgScene;
    root->SetCamera(cam);
    root->AddObject(skybox);
  //  root->AddObject(clouds);
    delete sl, ml;
    return true;
}
 void pxgWorld::SetSkyboxTexture(pxgTexture* tex)
 {
    if(tex==NULL)
        return;
    skybox->SetTexture(tex,PXG_TEXTURE0);
 }

bool pxgWorld::InitBullet()
{
    if(pw)
        return true;

    pw = new pxgPW;
    return pw->InitBullet();
}

void pxgWorld::AddRigidBody(pxgRigidBody* rb)
{
    if(rb&&pw)
        pw->AddRigidBody(rb);
}

void pxgWorld::SetCamera(pxgCamera* c)
{
    if(!c)
        return;
    cam = c;
    for(int i = 0;i<scenes.size();i++)
        scenes[i]->SetCamera(cam);
}

bool pxgWorld::InitPostProcess(int width, int height)
{
    if(frameQuad||postFramebuffer||postRenderTarget||postRenderbuffer)
    {
        PXG::Log("pxgWorld::InitPostProcess(): already initialised", WARN);
        return true;
    }

    PXG::glGenFramebuffers(1,&postFramebuffer);

    if(postFramebuffer==0)
    {
        PXG::Log("pxgWorld::InitPostProcess(): error generating framebuffer name", ERR);
        return false;
    }

    PXG::glBindFramebuffer(GL_FRAMEBUFFER,postFramebuffer);
    postRenderTarget = new pxgTexture();

    if(!postRenderTarget->CreateEmptyTexture(width, height,PXG_TEXTURE_RGB_UB))
        return false;

    postRenderTarget->Use(0);

    PXG::glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,postRenderTarget->GetName(), 0);

    PXG::glGenRenderbuffers(1,&postRenderbuffer);

    if(postRenderbuffer==0)
    {
        PXG::Log("pxgWorld::InitPostProcess(): error generating renderbufer name", ERR);
        return false;
    }

    PXG::glBindRenderbuffer(GL_RENDERBUFFER, postRenderbuffer);
    PXG::glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    PXG::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, postRenderbuffer);

    PXG::glBindFramebuffer(GL_FRAMEBUFFER,0);

    GLenum status;
    status = PXG::glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status!=GL_FRAMEBUFFER_COMPLETE)
    {
        PXG::Log("pxgWorld::InitPostProcess() error: framebuffer not complete", ERR);
        return false;
    }


    //init frame quad
    PXG::glGenVertexArrays(1,&frameVAO);

    if(frameVAO == 0)
    {
        PXG::Log("pxgWorld::InitPostProcess(): error generating VAO fro frame quad", ERR);
        return false;
    }
    PXG::glBindVertexArray(frameVAO);

    frameQuad = new pxgBuffer();
    frameQuad->Init();
    frameQuad->Use();

    using namespace glm;

    int x1 = -1;//-width/2;
    int x2 = 1;//width/2;
    int y1 = -1;//-height/2;
    int y2 = 1;//height/2;

    pxgVertex2dT vertices[] =
    {
        { vec2(x1,y2), vec2(0,0)},
        { vec2(x2,y2), vec2(1.0f,0)},
        { vec2(x2,y1), vec2(1.0f,1.0f)},
        { vec2(x2,y1), vec2(1.0f,1.0f)},
        { vec2(x1,y1), vec2(0,1.0f)},
        { vec2(y1,y2), vec2(0,0)}
    };

    frameQuad->SetData(VERTEX,vertices,sizeof(vertices),6);

    PXG::glVertexAttribPointer(PXG_POSITION, sizeof(glm::vec2)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex2dT), (void*)offsetof(pxgVertex2dT,pos));
    PXG::glEnableVertexAttribArray(PXG_POSITION);
    PXG::glVertexAttribPointer(PXG_TEXCOORD,sizeof(glm::vec2)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex2dT), (void*)offsetof(pxgVertex2dT, tc));
    PXG::glEnableVertexAttribArray(PXG_TEXCOORD);
    PXG::glBindVertexArray(NULL);


    postW = width;
    postH = height;

    return true;
}

bool pxgWorld::InitPicking(pxgScene *pickFrom, pxgShader* pShader, int width, int height)
{
    if(!pickFrom||!pShader)
        return false;

    PXG::glGenFramebuffers(1,&pickFramebuffer);

    if(pickFramebuffer==0)
    {
        PXG::Log("pxgWorld::InitPicking(): error generating framebuffer name", ERR);
        return false;
    }

    PXG::glBindFramebuffer(GL_FRAMEBUFFER,pickFramebuffer);
    pickBuffer = new pxgTexture();

    if(!pickBuffer->CreateEmptyTexture(width, height,PXG_TEXTURE_RGB_UB))
        return false;

    pickBuffer->Use(0);

    PXG::glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,pickBuffer->GetName(), 0);

    PXG::glGenRenderbuffers(1,&pickRenderbuffer);

    if(pickRenderbuffer==0)
    {
        PXG::Log("pxgWorld::InitPicking(): error generating renderbufer name", ERR);
        return false;
    }

    PXG::glBindRenderbuffer(GL_RENDERBUFFER, pickRenderbuffer);
    PXG::glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    PXG::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pickRenderbuffer);

    PXG::glBindFramebuffer(GL_FRAMEBUFFER,0);

    GLenum status;
    status = PXG::glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status!=GL_FRAMEBUFFER_COMPLETE)
    {
        PXG::Log("pxgWorld::InitPicking() error: framebuffer not complete", ERR);
        return false;
    }

    //init frame quad
    PXG::glGenVertexArrays(1,&pickVAO);

    if(pickVAO == 0)
    {
        PXG::Log("pxgWorld::InitPicking(): error generating VAO fro frame quad", ERR);
        return false;
    }
    PXG::glBindVertexArray(pickVAO);

    pickFrameQuad = new pxgBuffer();
    pickFrameQuad->Init();
    pickFrameQuad->Use();

    using namespace glm;

    int x1 = -1;//-width/2;
    int x2 = 1;//width/2;
    int y1 = -1;//-height/2;
    int y2 = 1;//height/2;

    pxgVertex2dT vertices[] =
    {
        { vec2(x1,y2), vec2(0,0)},
        { vec2(x2,y2), vec2(1.0f,0)},
        { vec2(x2,y1), vec2(1.0f,1.0f)},
        { vec2(x2,y1), vec2(1.0f,1.0f)},
        { vec2(x1,y1), vec2(0,1.0f)},
        { vec2(y1,y2), vec2(0,0)}
    };

    pickFrameQuad->SetData(VERTEX,vertices,sizeof(vertices),6);

    PXG::glVertexAttribPointer(PXG_POSITION, sizeof(glm::vec2)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex2dT), (void*)offsetof(pxgVertex2dT,pos));
    PXG::glEnableVertexAttribArray(PXG_POSITION);
    PXG::glVertexAttribPointer(PXG_TEXCOORD,sizeof(glm::vec2)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex2dT), (void*)offsetof(pxgVertex2dT, tc));
    PXG::glEnableVertexAttribArray(PXG_TEXCOORD);
    PXG::glBindVertexArray(NULL);

    pickScene = pickFrom;
    pickScene->SetCamera(cam);
    pickScene->SetWorld(this);

    pickShader = pShader;

    pickW = width;
    pickH = height;

    return true;
}

void pxgWorld::UpdatePickbuffer()
{
    if(!pickFramebuffer||!pickRenderbuffer||!pickBuffer||!pickScene)
        return;

    pickScene->Update();

    const GLenum draw_buffer = GL_COLOR_ATTACHMENT0;
    PXG::glDrawBuffers (1, &draw_buffer);

    PXG::glBindFramebuffer(GL_FRAMEBUFFER,pickFramebuffer);
    GLfloat bk[4];
    glGetFloatv(GL_COLOR_CLEAR_VALUE, bk);
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT,vp);
    glViewport(0,0,pickW,pickH);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    pickScene->DrawForPicking();

    PXG::glBindFramebuffer(GL_FRAMEBUFFER,0);
    glViewport(vp[0],vp[1],vp[2],vp[3]);
    glClearColor(bk[0],bk[1],bk[2],bk[3]);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
}

pxgObject* pxgWorld::PickObject(int _x, int _y, int w, int h)
{
    if(!pickingEnabled||!pickFramebuffer||!pickRenderbuffer||!pickBuffer||!pickScene)
        return NULL;

    UpdatePickbuffer();

    PXG::glBindFramebuffer(GL_FRAMEBUFFER, pickFramebuffer);
    glPixelStorei(GL_PACK_ALIGNMENT,1);
    unsigned char c[4] = {0,0,0,0};
    int x = _x*pickW/w;
    int y = _y*pickH/h;
    glReadPixels(x,pickH-y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,c);
    int id = c[0] + c[1]*256+c[2]*256*256;
    PXG::glBindFramebuffer(GL_FRAMEBUFFER,0);
    if(id>0)
        return pickScene->GetObjectByID(id-1);
    else
        return NULL;
}
