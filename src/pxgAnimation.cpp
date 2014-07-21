/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgAnimation.h>

#include <glm/gtx/matrix_interpolation.hpp>

pxgAnimation::pxgAnimation():speed(1.0f), length(0)
{}

void pxgAnimation::PushKeyframe(pxgKeyframe* kf)
{
    if(!kf)
        return;
    kframes.push_back(kf);
    length+=kf->length;
}

glm::mat4 pxgAnimation::GetTransformation(int _t)
{
    using namespace glm;
    int t;
    if(_t>length)
        t = length%_t;
    else
        t = length;
    if(t<0)
        return mat4(1);

    glm::mat4 mat1, mat2;
    int l = 0;
    int ind;
    for(int i = 0;i<kframes.size();i++)
    {
        if(l>t)
        {
            mat1 = kframes[i-1]->transformation;
            mat2 = kframes[i]->transformation;
            ind = i;
            break;
        }
        l+=kframes[i]->length;
    }
    float d = (l-t)/kframes[ind]->length;
    return glm::interpolate(mat1,mat2,d);
}
