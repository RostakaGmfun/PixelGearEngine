/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXG_WORLD_H
#define PXG_WORLD_H

#include <pxgNode.h>
#include <pxgCamera.h>
#include <vector>
#include <glm/fwd.hpp>
#include <pxgTerrain.h>

#include <pxgPhysics.h>

class pxgScene;
class pxgShader;
class pxgCamera;
class pxgObject;
class pxgTexture;
class pxgTerrain;
class pxgRigidBody;

typedef unsigned int GLuint;

class pxgWorld: public pxgNode
{
public:
    pxgWorld();
    bool InitSkybox();

    bool InitBullet();
    pxgPW* GetPW() const { return pw; }
    void AddRigidBody(pxgRigidBody* rb);

    void SetCamera(pxgCamera* c);

    bool InitPostProcess(int width, int height);
    void SetPostShader(pxgShader* s) {postShader = s; }
    pxgTexture* GetRTV() const { return postRenderTarget; }
    void EnablePostProcess(bool e) { postEnabled = e; }

    bool InitPicking(pxgScene* pickFrom, pxgShader* pickShader, int width, int height);
    pxgObject* PickObject(int x, int y, int w, int h);
    void EnablePicking(bool e) { pickingEnabled = e; }
    void EnablePickingDebug( bool e) { debugPicking = e; }
    pxgShader* GetPickShader() const { return pickShader; }

    void SetSkyboxTexture(pxgTexture* tex);
    void SetTerrain(pxgTerrain* t) { if(!t) return; terrain = t; t->SetWorld(this); }
    void Update();
    bool Render();
    const glm::mat4 GetView() const { return (cam!=NULL)?cam->GetView():glm::mat4(1); }
    const glm::mat4 GetProj() const { return (cam!=NULL)?cam->GetProj():glm::mat4(1); }
    const glm::mat4 GetVP() const { return (cam!=NULL)?cam->GetVP():glm::mat4(1); }
    pxgCamera* GetCamera() const { return cam; }
    void Destroy() {}
    void AddScene(pxgScene* scene);
    void RemoveScene(pxgScene* scene);
private:
    std::vector<pxgScene*> scenes;
    pxgObject* skybox;
    pxgShader* cloudShader;
    float cloudsMove;
    pxgScene* root;
    pxgCamera* cam;
    pxgTerrain* terrain;

    bool postEnabled;
    GLuint postFramebuffer;
    GLuint postRenderbuffer;
    GLuint frameVAO;
    pxgTexture* postRenderTarget;
    pxgShader* postShader;
    pxgBuffer* frameQuad;
    int postW;
    int postH;

    pxgTexture* pickBuffer;
    GLuint pickFramebuffer;
    GLuint pickRenderbuffer;
    GLuint pickVAO;
    pxgBuffer* pickFrameQuad;
    pxgScene* pickScene;
    pxgShader* pickShader;
    bool pickingEnabled;
    bool debugPicking;
    int pickW;
    int pickH;

    pxgPW* pw;

private:
    void UpdatePickbuffer();
};

#endif // PXG_WORLD_H
