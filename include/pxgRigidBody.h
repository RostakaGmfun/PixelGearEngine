/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXG_RIGIDBODY_H
#define PXG_RIGID_BODY_H

#include <btBulletDynamicsCommon.h>

class pxgPhysicsWorld;

typedef struct pxgRigidBodyProps
{
    float restitution;
    float mass;
    float friction;
    float rollingFriction;
    float angularDamping;
} pxgRBProps;


class pxgRigidBody
{
public:
    pxgRigidBody();
    ~pxgRigidBody();
    bool Init(pxgRBProps* p, btDefaultMotionState *ms, btCollisionShape* shape, short group, short mask);

    void ApplyForce(btVector3 f);
    void ApplyForceAtPos(btVector3 f, btVector3 p);
    void AddImpulse(btVector3 i);
    void AddImpulseAtPos(btVector3 i, btVector3 p);
    short GetGroup() const { return m_group; }
    short GetMask() const { return m_mask; }

    void Add();
    void Remove();

    btRigidBody* GetbtRB() const { return rBody; }

    btVector3 GetPosition();
    btQuaternion GetRotation();

private:
    btRigidBody* rBody;
    short m_group;
    short m_mask;
};


#endif //PXG_RIGID_BODY_H
