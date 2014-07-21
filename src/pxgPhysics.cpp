/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgPhysics.h>
#include <pxgTools.h>
#include <pxgRigidBody.h>

#include <btBulletWorldImporter.h>

pxgPW::~pxgPW()
{
    delete world;
    delete solver;
    delete dispatcher;
    delete collisionConf;
    delete broadphase;
}

bool pxgPW::InitBullet()
{
    world = NULL;
    broadphase = new btDbvtBroadphase();
    collisionConf = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConf);

    solver = new btSequentialImpulseConstraintSolver;

    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConf);
    PXG::Log("pxgPhysicsWorld::InitBullet(): successfully initialised");
    return true;
}

void pxgPW::SetGravity(btVector3 g)
{
    gravity = g;
    if(world)
        world->setGravity(gravity);
}

void pxgPW::AddRigidBody(pxgRigidBody* b)
{
    if(world&&b)
        world->addRigidBody(b->GetbtRB(),b->GetGroup(),b->GetMask());
}

void pxgPW::RemoveRigidBody(pxgRigidBody* b)
{
    if(world)
        world->removeRigidBody(b->GetbtRB());
}

void pxgPW::Update(float td)
{
    if(world)
        world->stepSimulation(td,1);
}

void pxgPW::LoadFormFile(std::string fname)
{
    if(!FileExists(fname))
    {
        PXG::Log("pxgPhysicsWOrld::LoadFromFile(): file does not exist: "+fname, ERR);
        return;
    }
    if(!world)
        return;
    //btBulletWorldImporter* fileLoader = new btBulletWorldImporter(world);
   // fileLoader->loadFile("Data/level1.bullet");
}

bool pxgPW::HasContact(pxgRigidBody *o)
{
    if(!o||!world)
        return false;
    int numManifolds = world->getDispatcher()->getNumManifolds();
    btCollisionObject* obj = o->GetbtRB();
        for (int i=0;i<numManifolds;i++)
        {
            btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
            const btCollisionObject* obA = contactManifold->getBody0();
            const btCollisionObject* obB = contactManifold->getBody1();
            if(obA==obj||obB==obj)
                return true;
        }
        return false;
}

bool pxgPW::HasContact(pxgRigidBody *o1, pxgRigidBody *o2)
{
    if(!o1||!o2||!world)
        return false;
    int numManifolds = world->getDispatcher()->getNumManifolds();
    btCollisionObject* obj1 = o1->GetbtRB();
    btCollisionObject* obj2 = o2->GetbtRB();
        for (int i=0;i<numManifolds;i++)
        {
            btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
            const btCollisionObject* obA = contactManifold->getBody0();
            const btCollisionObject* obB = contactManifold->getBody1();
            if((obA==obj1&&obB==obj2))
                return true;
            if  (obA==obj2&&obB==obj1)
                return true;
        }
        return false;
}

float pxgPW::CastRay(btVector3 start, btVector3 dir)
{
    if(!world)
        return 0;
    btCollisionWorld::ClosestRayResultCallback rayResult(start,dir);
    world->rayTest(start,dir,rayResult);

    if(rayResult.hasHit())
    {
        PXG::Log("asd");
        return (rayResult.m_hitPointWorld-rayResult.m_rayFromWorld).length();
    }
    return -1;
}

float pxgPW::CastRay(btVector3 start, btVector3 dir,unsigned short group)
{
    if(!world)
        return 0;
    btCollisionWorld::ClosestRayResultCallback rayResult(start,dir);
    rayResult.m_collisionFilterGroup = group;
    world->rayTest(start,dir,rayResult);

    if(rayResult.hasHit())
    {
        return (rayResult.m_hitPointWorld-start).length();
    }
    return -1;
}

