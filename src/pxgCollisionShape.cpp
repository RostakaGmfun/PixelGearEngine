/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgCollisionShape.h>

#include <pxgObject.h>
#include <pxgTools.h>
#include <pxgBuffer.h>

pxgCollisionShape::pxgCollisionShape(): shape(NULL), type(PXG_NULL_SHAPE)
{}

void pxgCollisionShape::InitBox(btVector3 sz)
{
    if(shape)
        delete shape;
    shape = new btBoxShape(sz);
}

void pxgCollisionShape::InitSphere(btScalar radius)
{
    if(shape)
        delete shape;
    shape = new btSphereShape(radius);
}

void pxgCollisionShape::InitStaticMesh(pxgObject *o)
{
    if(!o)
    {
        PXG::Log("pxgCollisionShape::InitStaticMesh() error: null object passed",ERR);
        return;
    }
  /*  void* verts = o->GetVBO()->GetData();
    if(!verts)
    {
        PXG::Log("pxgCollisionShape::InitStaticMesh() error: VBO data is null", ERR);
        return;
    }*/

    btTriangleMesh* mesh = new btTriangleMesh();

    switch(o->GetVertexLayout())
    {
        case PXG_VERTEX3D:
    {
        void* v = new pxgVertex3d[o->GetVBO()->GetDataSize()/sizeof(pxgVertex3d)];
        o->GetVBO()->GetData(v);
        pxgVertex3d* verts = (pxgVertex3d*)v;
            for(int i = 0;i<o->GetVBO()->GetSize();i+=3)
            {
                btVector3 A(verts[i].pos.x,verts[i].pos.y,verts[i].pos.z);
                btVector3 B(verts[i+1].pos.x,verts[i+1].pos.y,verts[i+1].pos.z);
                btVector3 C(verts[i+2].pos.x,verts[i+2].pos.y,verts[i+2].pos.z);
                mesh->addTriangle(A,B,C,false);
            }
            delete [] verts;
    }
            break;
        case PXG_VERTEX3DT:
    {
        void* v = new pxgVertex3dT[o->GetVBO()->GetDataSize()/sizeof(pxgVertex3dT)];
        o->GetVBO()->GetData(v);
        pxgVertex3dT* verts = (pxgVertex3dT*)v;
        for(int i = 0;i<o->GetVBO()->GetSize();i+=3)
        {
            btVector3 A(verts[i].pos.x,verts[i].pos.y,verts[i].pos.z);
            btVector3 B(verts[i+1].pos.x,verts[i+1].pos.y,verts[i+1].pos.z);
            btVector3 C(verts[i+2].pos.x,verts[i+2].pos.y,verts[i+2].pos.z);
            mesh->addTriangle(A,B,C,false);
        }
        delete [] verts;
    }
        break;
    case PXG_VERTEX3DTN:
    {
        void* v = new pxgVertex3dTN[o->GetVBO()->GetDataSize()/sizeof(pxgVertex3dTN)];
        o->GetVBO()->GetData(v);
        pxgVertex3dTN* verts = (pxgVertex3dTN*)v;
        for(int i = 0;i<o->GetVBO()->GetSize();i+=3)
        {
            btVector3 A(verts[i].pos.x,verts[i].pos.y,verts[i].pos.z);
            btVector3 B(verts[i+1].pos.x,verts[i+1].pos.y,verts[i+1].pos.z);
            btVector3 C(verts[i+2].pos.x,verts[i+2].pos.y,verts[i+2].pos.z);
            mesh->addTriangle(A,B,C,false);
        }
        delete [] verts;
    }
        break;
    default:
        PXG::Log("pxgCollisionMesh::InitStaticMesh() error: vertex layout not supported", ERR);
        return;
    }

   /* std::vector<pxgObject*> objects = o->GetChildMeshes();
    for(int i = 0;i<objects.size();i++)
    {
        pxgObject* obj = objects[i];
        switch(obj->GetVertexLayout())
        {
            case PXG_VERTEX3D:
        {
            void* v = new pxgVertex3d[obj->GetVBO()->GetDataSize()/sizeof(pxgVertex3d)];
            obj->GetVBO()->GetData(v);
            pxgVertex3d* verts = (pxgVertex3d*)v;
                for(int i = 0;i<obj->GetVBO()->GetSize();i+=3)
                {
                    btVector3 A(verts[i].pos.x,verts[i].pos.y,verts[i].pos.z);
                    btVector3 B(verts[i+1].pos.x,verts[i+1].pos.y,verts[i+1].pos.z);
                    btVector3 C(verts[i+2].pos.x,verts[i+2].pos.y,verts[i+2].pos.z);
                    mesh->addTriangle(A,B,C,false);
                }
                delete [] verts;
        }
                break;
            case PXG_VERTEX3DT:
        {
            void* v = new pxgVertex3dT[obj->GetVBO()->GetDataSize()/sizeof(pxgVertex3dT)];
            obj->GetVBO()->GetData(v);
            pxgVertex3dT* verts = (pxgVertex3dT*)v;
            for(int i = 0;i<obj->GetVBO()->GetSize();i+=3)
            {
                btVector3 A(verts[i].pos.x,verts[i].pos.y,verts[i].pos.z);
                btVector3 B(verts[i+1].pos.x,verts[i+1].pos.y,verts[i+1].pos.z);
                btVector3 C(verts[i+2].pos.x,verts[i+2].pos.y,verts[i+2].pos.z);
                mesh->addTriangle(A,B,C,false);
            }
            delete [] verts;
        }
            break;
        case PXG_VERTEX3DTN:
        {
            void* v = new pxgVertex3dTN[obj->GetVBO()->GetDataSize()/sizeof(pxgVertex3dTN)];
            obj->GetVBO()->GetData(v);
            pxgVertex3dTN* verts = (pxgVertex3dTN*)v;
            for(int i = 0;i<obj->GetVBO()->GetSize();i+=3)
            {
                btVector3 A(verts[i].pos.x,verts[i].pos.y,verts[i].pos.z);
                btVector3 B(verts[i+1].pos.x,verts[i+1].pos.y,verts[i+1].pos.z);
                btVector3 C(verts[i+2].pos.x,verts[i+2].pos.y,verts[i+2].pos.z);
                mesh->addTriangle(A,B,C,false);
            }
            delete [] verts;
        }
            break;
        default:
            PXG::Log("pxgCollisionMesh::InitStaticMesh() error: vertex layout not supported", ERR);
            return;
        }

    }*/

    shape = new btBvhTriangleMeshShape(mesh,false,true);
    PXG::Log("pxgCollisionShape::InitStaticMesh(): successfully created static collision shape from pxgObject");
    return;
}
