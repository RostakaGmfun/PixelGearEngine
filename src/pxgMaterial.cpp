/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgMaterial.h>
#include <pxgTools.h>

#include <glm/glm.hpp>

pxgMaterial::pxgMaterial(glm::vec3 d, glm::vec3 sp, glm::vec3 a, float s): diffuse(d), specular(sp), ambient(a)//, shininess(s)
{
    shininess = s;
}

pxgMaterial::pxgMaterial(): diffuse(glm::vec3(0)), ambient(0), specular(1),shininess(1000)
{}

glm::vec3 pxgMaterial::Diffuse() const { return diffuse; }
