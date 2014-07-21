/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXGCAMERA_H
#define PXGCAMERA_H

#include <pxgNode.h>

#include <glm/glm.hpp>

enum PXG_CAMERA_MODE
{
    FREE,
    LOOKAT,
    FPS,
    ARCBALL
};


class pxgCamera
{
public:
    pxgCamera();
    void Init(float width, float height, float fov, float near, float far);
    void SetLookPos(glm::vec3 l) { lookPos = l; }
    void Translate(glm::vec3 t);
    void SetType(PXG_CAMERA_MODE m) { mode = m; }
    void SetArcballRadius(float r) { arcRadius = r; }
    glm::vec3 GetTranslation() const { return translationVector;}
    glm::vec3 GetRotation() const { return rotationVector; }
    glm::vec3 GetLookPos() const { return lookPos; }
    void Rotate(glm::vec3 r);
    void SetArcHorDiapason(float min, float max) { minArcHor = min; maxArcHor = max; }
    void Move(float speed, bool upDir = true);
    void Transform(glm::mat4 m) { m_transform = m; }
    const glm::mat4 GetView() const { return View; }
    const glm::mat4 GetProj() const { return Proj; }
    const glm::mat4 GetVP() const { return Proj*View; }
    float GetNear() const { return near; }
    float GetFar() const { return far; }
    float GetWidth() const { return width; }
    float GetHeight()  const { return height; }
    void Update();

    enum PXG_RENDER_TARGET
    {
        FRAME = 1,
        TEXTURE,
        CUBE_TEXTURE
    };

private:
    glm::mat4 Proj, View, m_transform;
    float near, far, width, height;
    glm::vec3 translationVector, rotationVector, lookPos;
    PXG_CAMERA_MODE mode;
    float arcRadius;
    float maxArcHor;
    float minArcHor;

};

#endif //PXGCAMERA_H
