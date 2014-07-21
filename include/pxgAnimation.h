/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXG_ANIMATION_H
#define PXG_ANIMATION_H

#include <vector>

#include <pxgKeyframe.h>

class pxgAnimation
{
public:
    pxgAnimation();
    glm::mat4 GetTransformation(int t);
    void PushKeyframe(pxgKeyframe* kf);
    void SetSpeed(float s) { speed = s; }

private:
    std::vector<pxgKeyframe*> kframes;
    float speed;
    int length;
};

#endif //PXG_ANIMATION_H
