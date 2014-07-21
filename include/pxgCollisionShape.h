/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXG_COLLISIONSHAPE
#define PXG_COLLISIONSHAPE

#include <btBulletCollisionCommon.h>

class pxgObject;


enum PXG_COLLISION_SHAPE
{
    PXG_NULL_SHAPE,
    PXG_BOX_SHAPE,
    PXG_SPHERE_SHAPE,
    PXG_HULL_MESH_SHAPE,
    PXG_CONVEX_MESH_SHAPE,
    PXG_STATIC_MESH_SHAPE
};

class pxgCollisionShape
{
public:
    pxgCollisionShape();
    ~pxgCollisionShape();
    void InitBox(btVector3 size);
    void InitSphere(btScalar radius);
    void InitStaticMesh(pxgObject* o);
    btCollisionShape* GetShape() const { return shape; }
    PXG_COLLISION_SHAPE GetType() const { return type; }

private:
    PXG_COLLISION_SHAPE type;
    btCollisionShape* shape;


};

#endif //PXG_COLLISIONSHAPE
