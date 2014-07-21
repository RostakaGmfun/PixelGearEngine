/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXG_LIGHT_H
#define PXG_LIGHT_H

#include <pxgNode.h>

#include <glm/glm.hpp>

enum PXG_LIGHT_TYPE
{
    PXG_LIGHT_DIRECTIONAL,
    PXG_LIGHT_POINT,
    PXG_LIGHT_SPOT
};

class pxgLight: public pxgNode
{
public:
    pxgLight(PXG_LIGHT_TYPE t = PXG_LIGHT_DIRECTIONAL);
    bool Init(PXG_LIGHT_TYPE type);
    void SetLightColor(glm::vec3 c) { light_color = c; }
    glm::vec3 GetLightColor() const { return light_color; }
    void SetAttenuation(float a) { attenuation = a; }
    float GetAttenuation() const { return attenuation; }
    PXG_LIGHT_TYPE GetType() const { return type; }

    void Update();
    bool Render() {}
    void Destroy() {}

private:
    glm::vec3 light_color;
    glm::vec3 specular_color;
    glm::vec3 ambient_color;
    PXG_LIGHT_TYPE type;
    float intensity;
    float attenuation;
    float specularity;
};


#endif //PXG_LIGHT_H
