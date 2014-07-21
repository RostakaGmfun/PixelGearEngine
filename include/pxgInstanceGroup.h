/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXGINSTANCEGROUP_H
#define PXGINSTANCEGROUP_H

class pxgObject;

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct pxgInstance
{
    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 scaling;
    glm::mat4 transform;
    glm::mat4 m_transform;
};

class pxgInstanceGroup
{
    public:
    pxgInstanceGroup(pxgObject* o);
    void Update();
    void Render();
    void Destroy();

    int AddInstance(pxgInstance*);
    int GetInstanceCount();
    void RemoveInstance(int id);
    void RemoveInstance(pxgInstance*);

private:
    pxgObject* object;
    std::vector<pxgInstance*> instances;
};

#endif // PXGINSTANCEGROUP_H
