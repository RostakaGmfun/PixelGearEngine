/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgCamera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

using namespace glm;

#include <pxgTools.h>

pxgCamera::pxgCamera(): mode(FREE), arcRadius(10), minArcHor(-90.f), maxArcHor(90.f)
{
    translationVector = vec3(0,0,0);
    rotationVector = vec3(0,0,0);
}

void pxgCamera::Init(float w, float h, float fov, float n, float f)
{
    Proj = perspective(fov,w/h,n,f);
    width = w;
    height = h;
    near = n;
    far = f;
    Update();
}

void pxgCamera::Update()
{
    View = mat4(1);
    switch(mode)
    {
        case FREE:
        {
            View = rotate(View,rotationVector.x,vec3(1.0f,0.f,0.f));
            View = rotate(View,rotationVector.y,vec3(0.0f,1.f,0.f));
            View = rotate(View,rotationVector.z,vec3(0.0f,0.f,1.f));
            View = translate(View,translationVector);
            break;
        }
        case LOOKAT:
        {
        if(translationVector==lookPos)
            View = glm::lookAt(translationVector,lookPos+0.001f,vec3(0,1,0));
        else
            View = glm::lookAt(translationVector,lookPos,vec3(0,1,0));
            break;
        }
        case ARCBALL:
        {
            if(rotationVector.z>maxArcHor)
                rotationVector.z = maxArcHor;
            if(rotationVector.z<minArcHor)
                rotationVector.z = minArcHor;
            //translationVector.x = arcRadius* sinf(glm::radians(rotationVector.x))* cosf(glm::radians(rotationVector.z));
            //translationVector.y = arcRadius* sinf(glm::radians(rotationVector.x))* sinf(glm::radians(rotationVector.z));
            //translationVector.z = arcRadius* cosf(glm::radians(rotationVector.x));
            //glm::mat4 rot = glm::rotate(glm::mat4(),rotationVector.z,vec3(1,0,0));
            //rot = glm::rotate(rot,rotationVector.z,vec3(0,0,1));
            //View = glm::lookAt(translationVector,lookPos,vec3(0,1,0));
        //translationVector = (glm::rotate(glm::mat4(1),rotationVector.x,glm::cross(translationVector,lookPos))*(translationVector-lookPos))+lookPos;
       // translationVector = (glm::rotate(glm::mat4(1),rotationVector.z,vec3(0,1,0))*(translationVector-lookPos))+lookPos;
        //View = glm::lookAt(translationVector,lookPos,vec3(0,1,0));
            /*glm::mat4 rot = glm::yawPitchRoll(radians(rotationVector.z),glm::radians(rotationVector.x),0.f);
            vec3 t = glm::vec3(0,0,arcRadius);
            translationVector = lookPos+t;
            glm::vec4 up = vec4(vec3(0,1,0),1)*rot;
            View = glm::lookAt(translationVector,lookPos,glm::vec3(up));*/
            /*vec3 t = vec3(0,0,arcRadius);
            vec3 right = normalize(cross(vec3(0,1,0),t));
            vec3 up = normalize(cross(t,right));
            float th = rotationVector.x;
            float ph = rotationVector.z;
            mat4 rightRotMat = rotate(mat4(1),th,right);*/
            /*mat4 t = translate(mat4(1),-lookPos-vec3(0,0,arcRadius));
            //r = translate(r,vec3(0,0,arcRadius));
            mat4 r = rotate(t,rotationVector.x,vec3(1.0f,0.f,0.f));
            r = rotate(r,rotationVector.y,vec3(0.0f,1.f,0.f));
            r = rotate(r,rotationVector.z,vec3(0.0f,0.f,1.f));
           // r *= inverse(t);
            View = lookAt(vec3(r[3]),lookPos,vec3(0,1,0));*/
            /*vec4 p = vec4(0,1,0,0)*yawPitchRoll(rotationVector.x,0.f,rotationVector.z);
            vec3 t = vec3(p)+arcRadius+lookPos;
            View = lookAt(t,lookPos,vec3(0,1,0));*/
            translationVector.x = arcRadius* sinf(radians(rotationVector.x));
            translationVector.y = arcRadius* sinf(radians(rotationVector.z));//*cosf(radians(rotationVector.z));
            translationVector.z = arcRadius* -cosf(radians(rotationVector.x));
            View = lookAt(translationVector,vec3(0),vec3(0,1,0));
            View = translate(View,-lookPos);
            translationVector = vec3(View[3]);
            break;
        }
    }
    return;
}

void pxgCamera::Translate(vec3 t)
{
    translationVector = -t;
    return;
}

void pxgCamera::Rotate(vec3 r)
{
    rotationVector = r;
}

void pxgCamera::Move(float speed, bool upDir)
{

    static const float PI = 3.141596;
    float xMove = speed*sinf(rotationVector.z*PI/180.f);
    float yMove = speed*sinf(rotationVector.x*PI/180.f)*cosf(rotationVector.x*PI/180.f);
    float zMove = speed*-cosf(rotationVector.z*PI/180.f);

    translationVector.x += xMove;
    translationVector.y += -yMove;
    if(upDir)
        translationVector.z+= zMove;
}
