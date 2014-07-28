/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXGPARTICLESYSTEM_H
#define PXGPARTICLESYSTEM_H

class pxgBuffer;
class pxgMaterial;
class pxgScene;
class pxgShader;

#include <pxgTexture.h>

#include <pxgNode.h>
#include <glm/fwd.hpp>

class pxgParticleSystem: public pxgNode
{
public:
    pxgParticleSystem();
    void InitShader(std::string particleFunc);
    void InitBuffers();
    void SetTexture(pxgTexture,PXG_TEXTURE_STAGE);
    void SetColors(std::vector<glm::vec4> colors);
    void SetSize(float min, float max);
    void SetRotationRange(glm::vec3 min, glm::vec3 max);
    void SetStartVelocityRange(glm::vec3 min, glm::vec3 max);
    void SetLifetimeRange(float min, float max);
    void SetSpawnRadius(float r);
    void SetAccelerationRange(glm::vec3 min, glm::vec3 max);
    void SetSpawnRate(int particlesPerFrame);
    void SetMaxParticles(int max);

    void SetScene(pxgScene* s) { scene = s; }


    bool Render();
    void Update();
    void Destroy();

    static std::string pxgDefaultUpdateFunc;

private:
    GLuint feedbacks[2];
    GLuint buffers[2];

    pxgShader* updateShader;
    pxgShader* renderShader;

    pxgScene* scene;

    pxgTexture* texture;
    std::vector<glm::vec4> colors;
    float minSize;
    float maxSize;
    glm::vec3 minRot;
    glm::vec3 maxRot;
    glm::vec3 minStartVel;
    glm::vec3 maxStartVel;
    float minLifetime;
    float maxLifetime;
    float spawnRadius;
    glm::vec3 minAcceleration;
    glm::vec3 maxAcceleration;

    struct Particle
    {
        glm::vec3 pos;
        glm::vec3 velocity;
        glm::vec3 acceleration;
        glm::vec3 lifetime;
        glm::vec3 color;
    };



};

#endif // PXGPARTICLESYSTEM_H
