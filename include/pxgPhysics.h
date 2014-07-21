/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXG_PHYSICS_H
#define PXG_PHYSICS_H

#include <btBulletDynamicsCommon.h>
#include <string>


class pxgRigidBody;

typedef class pxgPhysicsWorld
{
public:

    ~pxgPhysicsWorld();
    bool InitBullet();
    void SetGravity(btVector3 g);
    void Update(float td);
    void AddRigidBody(pxgRigidBody *b);
    void RemoveRigidBody(pxgRigidBody* b);

    void LoadFormFile(std::string fname);

    bool HasContact(pxgRigidBody* obj);
    bool HasContact(pxgRigidBody* obj1, pxgRigidBody* obj2);
    float CastRay(btVector3 start, btVector3 dir);
    float CastRay(btVector3 start, btVector3 dir, unsigned short group);

private:

    btVector3 gravity;

    btDiscreteDynamicsWorld* world;
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConf;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;

private:
    void UpdateManifolds();

} pxgPW;

#endif //PXG_PHYSICS_H
