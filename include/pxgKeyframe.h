/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXG_KEYFRAME_H
#define PXG_KEYFRAME_H

#include <glm/mat4x4.hpp>

struct pxgKeyframe
{
    int length;
    glm::mat4 transformation;
};


#endif //PXG_KEYFRAME_H
