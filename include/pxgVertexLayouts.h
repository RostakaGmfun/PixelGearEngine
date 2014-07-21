/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXG_VLAYOUTS
#define PXG_VLAYOUTS

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

enum PXG_VERTEX_LAYOUT
{
    PXG_VERTEX2D,
    PXG_VERTEX2DT,
    PXG_VERTEX3D,
    PXG_VERTEX3DT,
    PXG_VERTEX3DN,
    PXG_VERTEX3DTN,
    PXG_VERTEX3DTANIM
};

struct pxgVertex2d
{
    glm::vec2 pos;
    glm::vec3 color;
};

struct pxgVertex2dT
{
    glm::vec2 pos;
    glm::vec2 tc;
};

struct pxgVertex3d
{
    glm::vec3 pos;
};

struct pxgVertex3dT
{
    glm::vec3 pos;
    glm::vec2 tc;
};

struct pxgVertex3dN
{
    glm::vec3 pos;
    glm::vec3 normal;
};

struct pxgVertex3dTN
{
    glm::vec3 pos;
    glm::vec2 tc;
    glm::vec3 normal;
};

struct pxgVertex3dTAnim
{
    glm::vec3 pos;
    glm::vec2 tc;
    glm::ivec4 indices;
    glm::vec4 weights;
};

#endif //PXG_LAYOUTS
