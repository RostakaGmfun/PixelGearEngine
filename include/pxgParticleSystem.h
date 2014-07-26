/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXGPARTICLESYSTEM_H
#define PXGPARTICLESYSTEM_H

class pxgBuffer;
class pxgTexture;
class pxgMaterial;
class pxgScene;

enum PXG_TEXTURE_STAGE;

#include <pxgNode.h>
#include <glm/fwd.hpp>

class pxgParticleSystem: public pxgNode
{
public:
    pxgParticleSystem();
    void InitShader(std::string particleFunc);
    void SetTexture(pxgTexture,PXG_TEXTURE_STAGE);
    void SetColors(std::vector<glm::vec4> colors);
    void SetSize(float min, float max);
    void SetRotationRange(glm::vec3 min, glm::vec3 max);
    void SetStartVelocityRange(glm::vec3 min, glm::vec3 max);
    void SetLifetimeRange(float min, float max);
    void SetSpawnRadius(float r);


    bool Render();
    void Update();
    void Destroy();

};

#endif // PXGPARTICLESYSTEM_H
