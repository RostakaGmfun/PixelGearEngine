/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgLight.h>

pxgLight::pxgLight(PXG_LIGHT_TYPE t):  light_color(1,1,1), ambient_color(1,1,1), specular_color(1,1,1), type(t)
{}

bool pxgLight::Init(PXG_LIGHT_TYPE t)
{
    type = t;
}

void pxgLight::Update()
{
    UpdateTransformation();
}
