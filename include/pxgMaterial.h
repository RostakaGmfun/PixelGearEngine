/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXGMATERIAL_H
#define PXGMATERIAL_H

#include <glm/glm.hpp>

class pxgMaterial
{
public:
    pxgMaterial();
    pxgMaterial(glm::vec3 diffuse,glm::vec3 specular, glm::vec3 ambient, float shinisness);
    glm::vec3 Diffuse() const;// { return diffuse; }
    glm::vec3 Specular() const { return specular; }
    glm::vec3 Ambient() const { return ambient; }
    float Shininess() const { return shininess; }

    void Diffuse(glm::vec3 d) { diffuse = d; }
    void Specular(glm::vec3 s) { specular = s; }
    void Ambient(glm::vec3 a) { ambient = a; }
    void Shininess(float s) { shininess = s; }

private:
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 ambient;
    float shininess;
};

#endif // PXGMATERIAL_H
