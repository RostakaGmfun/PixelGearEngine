/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgHUDObject.h>
#include <pxgHUDManager.h>
#include <pxgTexture.h>

#include <glm/gtc/matrix_transform.hpp>

pxgHUDObject::pxgHUDObject(): texture(NULL), type(PXG_HUD_TEXTURE), visible(true)
{
    transformation = glm::mat4(1);
    rotation = 0;
    translationVector = glm::vec2(0);
    size = glm::vec2(1);
    color = glm::vec4(0,0,0,1);
}

bool pxgHUDObject::Render()
{
    if(!texture)
        return false;
    texture->Use(0);
    return true;
}

void pxgHUDObject::Update()
{
    transformation = glm::mat4(1);
    transformation = glm::translate(transformation, glm::vec3(translationVector+glm::vec2(-size.x/2,size.y/2),0));
    //transformation = glm::translate(transformation,glm::vec3(size.x/2,size.y/2,0));
    transformation = glm::rotate(transformation,rotation,glm::vec3(0,0,1));
    transformation = glm::rotate(transformation,180.f,glm::vec3(1,0,0));
    transformation = glm::scale(transformation,glm::vec3(size,1));
}
