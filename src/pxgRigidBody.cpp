/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgRigidBody.h>
#include <pxgPhysics.h>
#include <pxgTools.h>

pxgRigidBody::pxgRigidBody()
{}

pxgRigidBody::~pxgRigidBody()
{
    if(rBody)
    {
        delete rBody->getMotionState();
        delete rBody;
    }
}

bool pxgRigidBody::Init(pxgRBProps* p, btDefaultMotionState *ms, btCollisionShape* s, short group, short mask)
{
    if(!s||!ms||!p)
    {
        PXG::Log("pxgRigidBody::Init() error: null pointers passed", ERR);
        return false;
    }


    btVector3 inertia(0,0,0);
    s->calculateLocalInertia(p->mass,inertia);
    btRigidBody::btRigidBodyConstructionInfo i(p->mass,ms,s,inertia);
    i.m_restitution = p->restitution;
    i.m_friction = p->friction;
    i.m_rollingFriction = p->rollingFriction;
    i.m_angularDamping = p->angularDamping;
    rBody = new btRigidBody(i);
    m_group = group;
    m_mask = mask;
    return true;
}

btVector3 pxgRigidBody::GetPosition()
{
    if(!rBody)
        return btVector3(0,0,0);
    btTransform trans;
    rBody->getMotionState()->getWorldTransform(trans);
    return trans.getOrigin();
}

btQuaternion pxgRigidBody::GetRotation()
{
    if(!rBody)
        return btQuaternion(0,0,0,1);
    btTransform trans;
    rBody->getMotionState()->getWorldTransform(trans);
    return trans.getRotation();
}

void pxgRigidBody::ApplyForce(btVector3 f)
{
    if(!rBody)
        return;
    rBody->activate(true);
    rBody->applyCentralForce(f);
}

void pxgRigidBody::AddImpulse(btVector3 i)
{
    if(!rBody)
        return;
    rBody->activate(true);
    rBody->applyCentralImpulse(i);
}

void pxgRigidBody::ApplyForceAtPos(btVector3 f, btVector3 p)
{
    if(!rBody)
        return;
    rBody->activate(true);
    rBody->applyForce(f,p);
}

void pxgRigidBody::AddImpulseAtPos(btVector3 i, btVector3 p)
{
    if(!rBody)
        return;
    rBody->activate(true);
    rBody->applyImpulse(i,p);
}
