/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgMeshLib.h>

#include <pxgTools.h>
#include <PXG.h>
#include <pxgBuffer.h>

#define GLM_PRECISION_HIGHP_FLOAT

#include <pxgVertexLayouts.h> //already includes required glm headers
#include <pxgObject.h>
#include <pxgTexture.h>
#include <pxgTexturePool.h>
#include <pxgLight.h>
#include <pxgLogicalObject.h>

#include <glm/gtx/normal.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <stdlib.h>


using namespace glm;

void pxgMeshLib::Init()
{
    m_cbMesh = CreateCubeMesh(1,1,1);
    m_cbnMesh = CreateCubeMeshWithNormals(1,1,1);
    m_plMesh = CreatePlaneMesh(1,1);
    m_plnMesh = CreatePlaneMeshWithNormals(1,1);
}

pxgObject* pxgMeshLib::CreateCubeMesh(float width, float height, float depth)
{
    pxgObject* obj = new pxgObject();
    pxgVertex3dT vertices[] = {

        {vec3(-width/2.f, -height/2.f, -depth/2.f),vec2(1.f,0.f)},
        {vec3(-width/2.f,  height/2.f, -depth/2.f),vec2(0.f,0.f)},
        {vec3( width/2.f, -height/2.f, -depth/2.f),vec2(1.f,1.f)},
        {vec3( width/2.f, -height/2.f, -depth/2.f),vec2(1.f,1.f)},
        {vec3(-width/2.f,  height/2.f, -depth/2.f),vec2(0.f,0.f)},
        {vec3(width/2.f,  height/2.f, -depth/2.f),vec2(0.f,1.f)},

        {vec3(-width/2.f, -height/2.f,  depth/2.f),vec2(0.f,0.f)},
        {vec3( width/2.f, -height/2.f,  depth/2.f),vec2(1.f,0.f)},
        {vec3(width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f)},
        {vec3(width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f)},
        {vec3(-width/2.f,  height/2.f,  depth/2.f),vec2(0.f,1.f)},
        {vec3( -width/2.f, -height/2.f,  depth/2.f),vec2(0.f,0.f)}, //correct

        {vec3(-width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f)},
        {vec3(-width/2.f,  height/2.f, -depth/2.f),vec2(1.f,0.f)},
        {vec3(-width/2.f, -height/2.f, -depth/2.f),vec2(0.f,0.f)},
        {vec3(-width/2.f, -height/2.f, -depth/2.f),vec2(0.f,0.f)},
        {vec3(-width/2.f, -height/2.f,  depth/2.f),vec2(0.f,1.f)},
        {vec3( -width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f)}, //correct

        {vec3(width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f)},
        {vec3(width/2.f,  -height/2.f, depth/2.f),vec2(1.f,0.f)},
        {vec3(width/2.f, -height/2.f, -depth/2.f),vec2(0.f,0.f)},
        {vec3(width/2.f, -height/2.f, -depth/2.f),vec2(0.f,0.f)},
        {vec3(width/2.f, height/2.f,  -depth/2.f),vec2(0.f,1.f)},
        {vec3(width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f)},

        {vec3(-width/2.f, -height/2.f, -depth/2.f),vec2(0.f,0.f)},
        {vec3( width/2.f, -height/2.f, -depth/2.f),vec2(1.f,0.f)},
        {vec3(width/2.f, -height/2.f,  depth/2.f),vec2(1.f,1.f)},
        {vec3(width/2.f, -height/2.f,  depth/2.f),vec2(1.f,1.f)},
        {vec3(-width/2.f, -height/2.f,  depth/2.f),vec2(0.f,1.f)},
        {vec3(-width/2.f, -height/2.f, -depth/2.f),vec2(0.f,0.f)},

        {vec3(-width/2.f,  height/2.f, -depth/2.f),vec2(0.f,0.f)},
        {vec3(-width/2.f,  height/2.f, depth/2.f),vec2(1.f,0.f)},
        {vec3(width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f)},
        {vec3(width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f)},
        {vec3(width/2.f,  height/2.f,  -depth/2.f),vec2(0.f,1.f)},
        {vec3(-width/2.f,  height/2.f, -depth/2.f),vec2(0.f,0.f)},
    };

    obj->Init();
    obj->SetVertexLayout(PXG_VERTEX3DT);
    obj->SetVertices((void*)vertices,sizeof(vertices),sizeof(vertices)/sizeof(pxgVertex3dT));
    return obj;
}

pxgObject* pxgMeshLib::CreatePlaneMesh(float width, float height)
{
    pxgObject *obj = new pxgObject;

    pxgVertex3dT data[] =
    {
        { vec3(-width/2.f, 0,height/2.f),vec2(0.f,0.f) },
        { vec3(width/2.f,  0,height/2.f),vec2(1.0f,0.f) },
        { vec3(width/2.f, 0, -height/2.f),vec2(1.0f,1.0f) },
        { vec3(-width/2.f,0,  -height/2.f),vec2(0.f,1.0f) }
    };

    GLuint elements[] =
    {
        0, 1, 2,
        2, 3, 0
    };
    obj->Init();
    obj->SetVertexLayout(PXG_VERTEX3DT);
    obj->SetVertices((void*)data,sizeof(data),sizeof(data)/sizeof(pxgVertex3dT));
    obj->SetIndices((void*)elements,sizeof(elements),sizeof(elements)/sizeof(GLuint));
    return obj;
}

pxgObject* pxgMeshLib::CreatePlaneMeshWithNormals(float width, float height)
{
    pxgObject *obj = new pxgObject;

    pxgVertex3dTN data[] =
    {
        { vec3(-width/2.f, 0,height/2.f), vec2(0.f,0.f), vec3(0,0,-1) },
        { vec3(width/2.f,  0,height/2.f), vec2(1.0f,0.f), vec3(0,0,-1) },
        { vec3(width/2.f, 0,-height/2.f), vec2(1.0f,1.0f), vec3(0,0,-1) },
        { vec3(-width/2.f, 0,-height/2.f), vec2(0.f,1.0f), vec3(0,0,-1) }
    };

    GLuint elements[] =
    {
        0, 1, 2,
        2, 3, 0
    };
    obj->Init();
    obj->SetVertexLayout(PXG_VERTEX3DTN);
    obj->SetVertices((void*)data,sizeof(data),sizeof(data)/sizeof(pxgVertex3dTN));
    obj->SetIndices((void*)elements,sizeof(elements),sizeof(elements)/sizeof(GLuint));
    return obj;
}


pxgObject* pxgMeshLib::CreateCubeMeshWithNormals(float width, float height, float depth)
{
    pxgObject* obj = new pxgObject();


    pxgVertex3dTN vertices[] = {
        {vec3(-width/2.f, -height/2.f, -depth/2.f),vec2(1.f,0.f), -triangleNormal(vec3(-width/2.f, -height/2.f, -depth/2.f),vec3( width/2.f, -height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f))},
        {vec3(-width/2.f,  height/2.f, -depth/2.f),vec2(0.f,0.f), -triangleNormal(vec3(-width/2.f, -height/2.f, -depth/2.f),vec3( width/2.f, -height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f))},
        {vec3( width/2.f, -height/2.f, -depth/2.f),vec2(1.f,1.f), -triangleNormal(vec3(-width/2.f, -height/2.f, -depth/2.f),vec3( width/2.f, -height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f))},
        {vec3(width/2.f,  -height/2.f, -depth/2.f),vec2(1.f,1.f), -triangleNormal(vec3(-width/2.f, -height/2.f, -depth/2.f),vec3( width/2.f, -height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f))},
        {vec3(-width/2.f,  height/2.f, -depth/2.f),vec2(0.f,0.f), -triangleNormal(vec3(-width/2.f, -height/2.f, -depth/2.f),vec3( width/2.f, -height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f))},
        {vec3(width/2.f,  height/2.f, -depth/2.f),vec2(0.f,1.f), -triangleNormal(vec3(-width/2.f, -height/2.f, -depth/2.f),vec3( width/2.f, -height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f))},

        {vec3(-width/2.f, -height/2.f,  depth/2.f),vec2(0.f,0.f), triangleNormal(vec3(-width/2.f, -height/2.f,  depth/2.f),vec3( width/2.f, -height/2.f,  depth/2.f),vec3(width/2.f,  height/2.f,  depth/2.f))},
        {vec3( width/2.f, -height/2.f,  depth/2.f),vec2(1.f,0.f), triangleNormal(vec3(-width/2.f, -height/2.f,  depth/2.f),vec3( width/2.f, -height/2.f,  depth/2.f),vec3(width/2.f,  height/2.f,  depth/2.f))},
        {vec3(width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f), triangleNormal(vec3(-width/2.f, -height/2.f,  depth/2.f),vec3( width/2.f, -height/2.f,  depth/2.f),vec3(width/2.f,  height/2.f,  depth/2.f))},
        {vec3(width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f), triangleNormal(vec3(-width/2.f, -height/2.f,  depth/2.f),vec3( width/2.f, -height/2.f,  depth/2.f),vec3(width/2.f,  height/2.f,  depth/2.f))},
        {vec3(-width/2.f,  height/2.f,  depth/2.f),vec2(0.f,1.f), triangleNormal(vec3(-width/2.f, -height/2.f,  depth/2.f),vec3( width/2.f, -height/2.f,  depth/2.f),vec3(width/2.f,  height/2.f,  depth/2.f))},
        {vec3( -width/2.f, -height/2.f,  depth/2.f),vec2(0.f,0.f), triangleNormal(vec3(-width/2.f, -height/2.f,  depth/2.f),vec3( width/2.f, -height/2.f,  depth/2.f),vec3(width/2.f,  height/2.f,  depth/2.f))},

        {vec3(-width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f), triangleNormal(vec3(-width/2.f,  height/2.f,  depth/2.f),vec3(-width/2.f,  height/2.f, -depth/2.f),vec3(-width/2.f, -height/2.f, -depth/2.f))},
        {vec3(-width/2.f,  height/2.f, -depth/2.f),vec2(1.f,0.f), triangleNormal(vec3(-width/2.f,  height/2.f,  depth/2.f),vec3(-width/2.f,  height/2.f, -depth/2.f),vec3(-width/2.f, -height/2.f, -depth/2.f))},
        {vec3(-width/2.f, -height/2.f, -depth/2.f),vec2(0.f,0.f), triangleNormal(vec3(-width/2.f,  height/2.f,  depth/2.f),vec3(-width/2.f,  height/2.f, -depth/2.f),vec3(-width/2.f, -height/2.f, -depth/2.f))},
        {vec3(-width/2.f, -height/2.f, -depth/2.f),vec2(0.f,0.f), triangleNormal(vec3(-width/2.f,  height/2.f,  depth/2.f),vec3(-width/2.f,  height/2.f, -depth/2.f),vec3(-width/2.f, -height/2.f, -depth/2.f))},
        {vec3(-width/2.f, -height/2.f,  depth/2.f),vec2(0.f,1.f), triangleNormal(vec3(-width/2.f,  height/2.f,  depth/2.f),vec3(-width/2.f,  height/2.f, -depth/2.f),vec3(-width/2.f, -height/2.f, -depth/2.f))},
        {vec3( -width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f), triangleNormal(vec3(-width/2.f,  height/2.f,  depth/2.f),vec3(-width/2.f,  height/2.f, -depth/2.f),vec3(-width/2.f, -height/2.f, -depth/2.f))},

        {vec3(width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f), -triangleNormal(vec3(width/2.f,  height/2.f,  depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f, -height/2.f, -depth/2.f))},
        {vec3(width/2.f,  -height/2.f, depth/2.f),vec2(1.f,0.f), -triangleNormal(vec3(width/2.f,  height/2.f,  depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f, -height/2.f, -depth/2.f))},
        {vec3(width/2.f, -height/2.f, -depth/2.f),vec2(0.f,0.f), -triangleNormal(vec3(width/2.f,  height/2.f,  depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f, -height/2.f, -depth/2.f))},
        {vec3(width/2.f, -height/2.f, -depth/2.f),vec2(0.f,0.f), -triangleNormal(vec3(width/2.f,  height/2.f,  depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f, -height/2.f, -depth/2.f))},
        {vec3(width/2.f, height/2.f,  -depth/2.f),vec2(0.f,1.f), -triangleNormal(vec3(width/2.f,  height/2.f,  depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f, -height/2.f, -depth/2.f))},
        {vec3(width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f), -triangleNormal(vec3(width/2.f,  height/2.f,  depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f, -height/2.f, -depth/2.f))},

        {vec3(-width/2.f, -height/2.f, -depth/2.f),vec2(0.f,0.f), triangleNormal(vec3(-width/2.f, -height/2.f, -depth/2.f),vec3( width/2.f, -height/2.f, -depth/2.f),vec3(width/2.f, -height/2.f,  depth/2.f))},
        {vec3( width/2.f, -height/2.f, -depth/2.f),vec2(1.f,0.f), triangleNormal(vec3(-width/2.f, -height/2.f, -depth/2.f),vec3( width/2.f, -height/2.f, -depth/2.f),vec3(width/2.f, -height/2.f,  depth/2.f))},
        {vec3(width/2.f, -height/2.f,  depth/2.f),vec2(1.f,1.f), triangleNormal(vec3(-width/2.f, -height/2.f, -depth/2.f),vec3( width/2.f, -height/2.f, -depth/2.f),vec3(width/2.f, -height/2.f,  depth/2.f))},
        {vec3(width/2.f, -height/2.f,  depth/2.f),vec2(1.f,1.f), triangleNormal(vec3(-width/2.f, -height/2.f, -depth/2.f),vec3( width/2.f, -height/2.f, -depth/2.f),vec3(width/2.f, -height/2.f,  depth/2.f))},
        {vec3(-width/2.f, -height/2.f,  depth/2.f),vec2(0.f,1.f), triangleNormal(vec3(-width/2.f, -height/2.f, -depth/2.f),vec3( width/2.f, -height/2.f, -depth/2.f),vec3(width/2.f, -height/2.f,  depth/2.f))},
        {vec3(-width/2.f, -height/2.f, -depth/2.f),vec2(0.f,0.f), triangleNormal(vec3(-width/2.f, -height/2.f, -depth/2.f),vec3( width/2.f, -height/2.f, -depth/2.f),vec3(width/2.f, -height/2.f,  depth/2.f))},

         /*{vec3(-width/2.f,  height/2.f, -depth/2.f),vec2(0.f,0.f)},
        {vec3(-width/2.f,  height/2.f, depth/2.f),vec2(1.f,0.f)},
        {vec3(width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f)},
        {vec3(width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f)},
        {vec3(width/2.f,  height/2.f,  -depth/2.f),vec2(0.f,1.f)},
        {vec3(-width/2.f,  height/2.f, -depth/2.f),vec2(0.f,0.f)},*/


        {vec3(-width/2.f,  height/2.f, -depth/2.f),vec2(0.f,0.f), -triangleNormal(vec3(-width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f,  depth/2.f))},
        {vec3(-width/2.f,  height/2.f, depth/2.f),vec2(1.f,0.f), -triangleNormal(vec3(-width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f,  depth/2.f))},
        {vec3(width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f), -triangleNormal(vec3(-width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f,  depth/2.f))},
        {vec3(width/2.f,  height/2.f,  depth/2.f),vec2(1.f,1.f), -triangleNormal(vec3(-width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f,  depth/2.f))},
        {vec3(width/2.f,  height/2.f,  -depth/2.f),vec2(0.f,1.f), -triangleNormal(vec3(-width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f,  depth/2.f))},
        {vec3(-width/2.f,  height/2.f, -depth/2.f),vec2(0.f,0.f), -triangleNormal(vec3(-width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f, -depth/2.f),vec3(width/2.f,  height/2.f,  depth/2.f))},
    };

    obj->Init();
    obj->SetVertexLayout(PXG_VERTEX3DTN);
    obj->SetVertices((void*)vertices,sizeof(vertices),sizeof(vertices)/sizeof(pxgVertex3dTN));
    return obj;
}

pxgObject* pxgMeshLib::GetUnitCubeWithNormals() { return CreateCubeMeshWithNormals(1,1,1); }
pxgObject* pxgMeshLib::GetUnitCube() { return CreateCubeMesh(1,1,1); }

pxgObject* pxgMeshLib::GetUnitPlaneWithNormals() { return CreatePlaneMeshWithNormals(1,1); }
pxgObject* pxgMeshLib::GetUnitPlane() { return CreatePlaneMeshWithNormals(1,1); }

pxgObject* pxgMeshLib::LoadFromFile(std::string fname, PXG_MODEL_TYPE type)
{
    if(!FileExists(fname))
    {
        PXG::Log("pxgMeshLib::LoadFromFile() error: no such file: "+fname);
        return NULL;
    }
    switch(type)
    {
        case PXG_WAVEFRONT_OBJ:
            return LoadObjMulti(fname);
        break;
        case PXG_3DS:
            return Load3DS(fname);
        break;
        default:
            break;
    }
    return NULL;
}

pxgObject* pxgMeshLib::LoadObj(std::string fname)
{
    using namespace std;
    ifstream f(fname);
    vector<string> lines;
    while(f.good())
    {
        string l;
        getline(f,l);
        lines.push_back(l);
    }

    vector<glm::vec3> positions, normals;
    vector<glm::vec2> texcoords;
    vector<glm::ivec3> faces;

    for(int i = 0;i<lines.size();i++)
    {
        switch(lines[i][0])
        {
            case '#':
            {
                //PXG::Log("comment");
                break;
            }
            case 'v':
            {
                switch(lines[i][1])
                {
                    case ' ': // positions
                    {
                        stringstream ss(lines[i]);
                        string v;
                        getline(ss,v,' '); //v
                        //PXG::Log(v);
                        glm::vec3 p;
                        getline(ss,v,' '); //x
                        p.x = stof(v);
                        //PXG::Log(v);
                        getline(ss,v,' '); //y
                        p.y = stof(v);
                        //PXG::Log(v);
                        getline(ss,v,' '); //z
                        p.z = stof(v);
                        //PXG::Log(v);
                        positions.push_back(p);
                        break;
                    }
                    case 'n': //normals
                    {
                        stringstream ss(lines[i]);
                        string v;
                        getline(ss,v,' '); //vn
                        //PXG::Log(v);
                        glm::vec3 p;
                        getline(ss,v,' '); //x
                        p.x = stof(v);
                        //PXG::Log(v);
                        getline(ss,v,' '); //y
                        p.y = stof(v);
                        //PXG::Log(v);
                        getline(ss,v,' '); //z
                        p.z = stof(v);
                        //PXG::Log(v);
                        normals.push_back(p);
                        break;
                    }
                    case 't': //texcoords
                    {
                        stringstream ss(lines[i]);
                        string v;
                        getline(ss,v,' '); //vt
                        //PXG::Log(v);
                        glm::vec2 p;
                        getline(ss,v,' '); //x
                        p.x = stof(v);
                       // PXG::Log(v);
                        getline(ss,v,' '); //y
                        p.y = stof(v);
                       // PXG::Log(v);
                        texcoords.push_back(p);
                        break;
                    }
                }
                break;
            }
            case 'f': //faces
            {
                stringstream ss(lines[i]);
                string v;
                getline(ss,v,' '); //v
                //PXG::Log(lines[i]);
                glm::ivec3 f0,f1,f2;
                std::string face0,face1,face2;
                getline(ss,face0, ' ');
                getline(ss,face1, ' ');
                getline(ss,face2, ' ');
                //PXG::Log(face0);
                stringstream ss0(face0);
                stringstream ss1(face1);
                stringstream ss2(face2);

                getline(ss0,v,'/');
                if(!v.empty())
                    f0.x = stoi(v);
                //PXG::Log(v);
                getline(ss0,v,'/');
                if(!v.empty())
                    f0.y = stoi(v);
                //PXG::Log(v);
                getline(ss0,v,'/');
                if(!v.empty())
                    f0.z = stoi(v);
                //PXG::Log(v);
                faces.push_back(f0);

                getline(ss1,v,'/');
                if(!v.empty())
                    f1.x = stoi(v);
                //PXG::Log(v);
                getline(ss1,v,'/');
                if(!v.empty())
                    f1.y = stoi(v);
                //PXG::Log(v);
                getline(ss1,v,'/');
                if(!v.empty())
                    f1.z = stoi(v);
                //PXG::Log(v);
                faces.push_back(f1);

                getline(ss2,v,'/');
                if(!v.empty())
                    f2.x = stoi(v);
                //PXG::Log(v);
                getline(ss2,v,'/');
                if(!v.empty())
                    f2.y = stoi(v);
                //PXG::Log(v);
                getline(ss2,v,'/');
                if(!v.empty())
                    f2.z = stoi(v);
                //PXG::Log(v);
                faces.push_back(f2);
                break;
            }
        }
        //PXG::Log(lines[i]);
    }
    bool n,t;
    PXG_VERTEX_LAYOUT layout;
    if(positions.size()<=0)
    {
        PXG::Log("pxgMeshLib::LoadObj() error: no vertices found", ERR);
        return NULL;
    }
    if(normals.size()<=0)
        n = false;
    else
        n = true;
    if(texcoords.size()<=0)
        t = false;
    else
        t = true;
    if(n&&t)
        layout = PXG_VERTEX3DTN;
    if(n&&!t)
        layout = PXG_VERTEX3DN;
    if(!n&&t)
        layout = PXG_VERTEX3DT;
    if(!n&&!t)
        layout = PXG_VERTEX3D;

    int sz;


    pxgObject* obj = new pxgObject;
    obj->Init();

    switch(layout)
    {
        case PXG_VERTEX3DTN:
        {
            pxgVertex3dTN *vtn;
            vtn = new pxgVertex3dTN[faces.size()];
            sz = FillVTN(positions,texcoords,normals,faces, vtn);
            obj->SetVertexLayout(PXG_VERTEX3DTN);
            obj->SetVertices(vtn,sz*sizeof(pxgVertex3dTN),sz);
            break;
        }
        case PXG_VERTEX3DN:
        {
            pxgVertex3dN* vn;
            vn = new pxgVertex3dN[faces.size()];
            sz = FillVN(positions, normals,faces, vn);
            obj->SetVertexLayout(PXG_VERTEX3DN);
            obj->SetVertices(vn,sz*sizeof(pxgVertex3dN),sz);
            break;
        }
        case PXG_VERTEX3DT:
        {
            pxgVertex3dT* vt;
            vt = new pxgVertex3dT[faces.size()];
            sz = FillVT(positions,texcoords,faces, vt);
            obj->SetVertexLayout(PXG_VERTEX3DT);
            obj->SetVertices(vt,sz*sizeof(pxgVertex3dT),sz);
            break;
        }
        case PXG_VERTEX3D:
        {
            pxgVertex3d* v3d;
            v3d = new pxgVertex3d[faces.size()];
            sz = FillV3D(positions, faces, v3d);
            obj->SetVertexLayout(PXG_VERTEX3D);
            obj->SetVertices(v3d,sz*sizeof(pxgVertex3d),sz);
            break;
        }
    }

    /*for(int i = 0;i<sz;i++)
        PXG::Log(std::to_string(vtn[i].pos.x)+" "+std::to_string(vtn[i].pos.y)+" "+std::to_string(vtn[i].pos.z)+
        " | "+std::to_string(vtn[i].tc.x)+" "+std::to_string(vtn[i].tc.y)+
        " | "+std::to_string(vtn[i].normal.x)+" "+std::to_string(vtn[i].normal.y)+" "+std::to_string(vtn[i].normal.z));*/

    return obj;
}

pxgObject* pxgMeshLib::LoadObjMulti(std::string fname)
{
    using namespace std;

    int numObjects = 0;

    struct obj_entity
    {
        vector<glm::vec3> positions, normals;
        vector<glm::vec2> texcoords;
        vector<glm::ivec3> faces;
        string matName;
        string name;

        obj_entity()/*: mat(NULL)*/ {}
    };

    vector<obj_entity*> objects;
    vector<obj_material*> materials;

    ifstream f(fname);
    vector<string> lines;
    while(f.good())
    {
        string l;
        getline(f,l);
        lines.push_back(l);
    }

    string mtl_file;

    vector<glm::vec3> positions, normals;
    vector<glm::vec2> texcoords;

    objects.push_back(new obj_entity());

    for(int i = 0;i<lines.size();i++)
    {
        switch(lines[i][0])
        {
            case '#':
            {
                //PXG::Log("comment");
                break;
            }
            case 'v':
            {
                switch(lines[i][1])
                {
                    case ' ': // positions
                    {
                        stringstream ss(lines[i]);
                        string v;
                        getline(ss,v,' '); //v
                        //PXG::Log(v);
                        glm::vec3 p;
                        getline(ss,v,' '); //x
                        PXG::Log(v);
                        p.x = stof(std::string(v).c_str());
                        //PXG::Log(std::to_string(p.x)+"");
                        getline(ss,v,' '); //y
                        p.y = stof(v);
                        //PXG::Log(v);
                        getline(ss,v,' '); //z
                        p.z = stof(v);
                        //PXG::Log(v);
                        positions.push_back(p);
                        break;
                    }
                    case 'n': //normals
                    {
                        stringstream ss(lines[i]);
                        string v;
                        getline(ss,v,' '); //vn
                        //PXG::Log(v);
                        glm::vec3 p;
                        getline(ss,v,' '); //x
                        p.x = stof(v);
                        //PXG::Log(v);
                        getline(ss,v,' '); //y
                        p.y = stof(v);
                        //PXG::Log(v);
                        getline(ss,v,' '); //z
                        p.z = stof(v);
                        //PXG::Log(v);
                        normals.push_back(p);
                        break;
                    }
                    case 't': //texcoords
                    {
                        stringstream ss(lines[i]);
                        string v;
                        getline(ss,v,' '); //vt
                        //PXG::Log(v);
                        glm::vec2 p;
                        getline(ss,v,' '); //x
                        p.x = stof(v);
                       // PXG::Log(v);
                        getline(ss,v,' '); //y
                        p.y = stof(v);
                       // PXG::Log(v);
                        texcoords.push_back(p);
                        break;
                    }
                }
                break;
            }
            case 'o':
            {
                stringstream ss(lines[i]);
                string objName;
                getline(ss,objName,' ');
                getline(ss,objName,' ');
                //PXG::Log(objName);
                if(objects[numObjects]->name.empty()) //the name of first object
                {
                    objects[numObjects]->name = objName;
                    //PXG::Log("existing: "+objName);
                    break;
                }
                else //create new object
                {
                    objects.push_back(new obj_entity());
                    numObjects++;
                    objects[numObjects]->name = objName;
                    //PXG::Log(objName);
                    break;
                }
                break;
            }
            case 'm':
            {
                stringstream ss(lines[i]);
                string matName;
                getline(ss,matName,' ');
                if(matName!="mtllib")
                {
                    PXG::Log("pxgMeshLib::LoadObjMulti() warning: unknown token: "+matName,WARN);
                    break;
                }
                getline(ss,matName,' ');
                mtl_file = matName;
                string rel_path = GetPath(fname);
                if(!FileExists(rel_path+matName))
                {
                    PXG::Log("pxgMeshLib() error: material library not found: "+matName,ERR);
                    break;
                }
                materials = LoadMTL(rel_path+matName);
                break;
            }
            case 'u':
            {
                stringstream ss(lines[i]);
                string matName;
                getline(ss,matName,' ');
                if(matName!="usemtl")
                {
                    PXG::Log("pxgMeshLib::LoadObjMulti() warning: unknown token: "+matName,WARN);
                    break;
                }
                getline(ss,matName,' ');
                /*if(objects[numObjects]->mat!=NULL)
                    if(objects[numObjects]->mat->name.empty())
                        objects[numObjects]->mat->name = matName;
                    else
                        {
                            PXG::Log("pxgMeshLib::LoadObjMulti() warning: object with name "+objects[numObjects]->name+" already has material with name "+objects[numObjects]->mat->name,WARN);
                            break;
                        }*/
                objects[numObjects]->matName = matName;
                break;
            }
            case 'f': //faces
            {
                stringstream ss(lines[i]);
                string v;
                getline(ss,v,' '); //v
                //PXG::Log(lines[i]);
                glm::ivec3 f0,f1,f2;
                std::string face0,face1,face2;
                getline(ss,face0, ' ');
                getline(ss,face1, ' ');
                getline(ss,face2, ' ');
                //PXG::Log(face0);
                stringstream ss0(face0);
                stringstream ss1(face1);
                stringstream ss2(face2);

                getline(ss0,v,'/');
                if(!v.empty())
                    f0.x = stoi(v);
                //PXG::Log(v);
                getline(ss0,v,'/');
                if(!v.empty())
                    f0.y = stoi(v);
                //PXG::Log(v);
                getline(ss0,v,'/');
                if(!v.empty())
                    f0.z = stoi(v);
                //PXG::Log(v);
                objects[numObjects]->faces.push_back(f0);

                getline(ss1,v,'/');
                if(!v.empty())
                    f1.x = stoi(v);
                //PXG::Log(v);
                getline(ss1,v,'/');
                if(!v.empty())
                    f1.y = stoi(v);
                //PXG::Log(v);
                getline(ss1,v,'/');
                if(!v.empty())
                    f1.z = stoi(v);
                //PXG::Log(v);
                objects[numObjects]->faces.push_back(f1);

                getline(ss2,v,'/');
                if(!v.empty())
                    f2.x = stoi(v);
                //PXG::Log(v);
                getline(ss2,v,'/');
                if(!v.empty())
                    f2.y = stoi(v);
                //PXG::Log(v);
                getline(ss2,v,'/');
                if(!v.empty())
                    f2.z = stoi(v);
                //PXG::Log(v);
                objects[numObjects]->faces.push_back(f2);
                break;
            }
        }
        //PXG::Log(lines[i]);
    }


    bool n,t;
    PXG_VERTEX_LAYOUT layout;
    if(positions.size()<=0)
    {
        PXG::Log("pxgMeshLib::LoadObjMulti() error: no vertices found", ERR);
        return NULL;
    }
    if(normals.size()<=0)
        n = false;
    else
        n = true;
    if(texcoords.size()<=0)
        t = false;
    else
        t = true;
    if(n&&t)
        layout = PXG_VERTEX3DTN;
    if(n&&!t)
        layout = PXG_VERTEX3DN;
    if(!n&&t)
        layout = PXG_VERTEX3DT;
    if(!n&&!t)
        layout = PXG_VERTEX3D;

    int sz;
    pxgObject *obj = new pxgObject;
    obj->Init();
    //PXG::Log(objects[0]->name);
    //PXG::Log(to_string(objects[0]->faces.size()));

    obj_material* mat = NULL;
    for(int j = 0;j<materials.size();j++)
    {
        if(materials[j]!=NULL)
            if(materials[j]->name == objects[0]->matName)
            {
                mat = materials[j];
                break;
            }
    }
    if(mat!=NULL)
    {
        obj->SetAmbient(mat->ambient);
        obj->SetDiffuse(mat->diffuse);
        obj->SetSpecular(mat->specular);
        if(mat->texture!=NULL)
        {
            obj->SetTexture(mat->texture,PXG_TEXTURE0);
        }
    }

    switch(layout)
    {
        case PXG_VERTEX3DTN:
        {
            pxgVertex3dTN *vtn;
            vtn = new pxgVertex3dTN[objects[0]->faces.size()];
            sz = FillVTN(positions,texcoords,normals,objects[0]->faces, vtn);
            obj->SetVertexLayout(PXG_VERTEX3DTN);
            obj->SetVertices(vtn,sz*sizeof(pxgVertex3dTN),sz);
            break;
        }
        case PXG_VERTEX3DN:
        {
            pxgVertex3dN* vn;
            vn = new pxgVertex3dN[objects[0]->faces.size()];
            sz = FillVN(objects[0]->positions, objects[0]->normals,objects[0]->faces, vn);
            obj->SetVertexLayout(PXG_VERTEX3DN);
            obj->SetVertices(vn,sz*sizeof(pxgVertex3dN),sz);
            break;
        }
        case PXG_VERTEX3DT:
        {
            pxgVertex3dT* vt;
            vt = new pxgVertex3dT[objects[0]->faces.size()];
            sz = FillVT(objects[0]->positions,objects[0]->texcoords,objects[0]->faces, vt);
            obj->SetVertexLayout(PXG_VERTEX3DT);
            obj->SetVertices(vt,sz*sizeof(pxgVertex3dT),sz);
            break;
        }
        case PXG_VERTEX3D:
        {
            pxgVertex3d* v3d;
            v3d = new pxgVertex3d[objects[0]->faces.size()];
            sz = FillV3D(objects[0]->positions, objects[0]->faces, v3d);
            obj->SetVertexLayout(PXG_VERTEX3D);
            obj->SetVertices(v3d,sz*sizeof(pxgVertex3d),sz);
            break;
        }
    }

    for(int i = 1;i<objects.size();i++)
    {
        if(objects[i]==NULL)
        {
            continue;
        }
        else
        {
            n = false;
            t  =false;
            PXG_VERTEX_LAYOUT layout;
            if(positions.size()<=0)
            {
                PXG::Log("pxgMeshLib::LoadObjMulti() error: no vertices found", ERR);
                continue;
            }
            if(normals.size()<=0)
                n = false;
            else
                n = true;
            if(texcoords.size()<=0)
                t = false;
            else
                t = true;
            if(n&&t)
                layout = PXG_VERTEX3DTN;
            if(n&&!t)
                layout = PXG_VERTEX3DN;
            if(!n&&t)
                layout = PXG_VERTEX3DT;
            if(!n&&!t)
                layout = PXG_VERTEX3D;

            int sz;
            pxgObject *o = new pxgObject;
            obj->AddChildMesh(o);
            //obj = o;
            o->Init();
            //PXG::Log(objects[i]->name);
            //PXG::Log(to_string(objects[i]->faces.size()));

            switch(layout)
            {
                case PXG_VERTEX3DTN:
                {
                    pxgVertex3dTN *vtn;
                    vtn = new pxgVertex3dTN[objects[i]->faces.size()];
                    sz = FillVTN(positions,texcoords,normals,objects[i]->faces, vtn);
                    o->SetVertexLayout(PXG_VERTEX3DTN);
                    o->SetVertices(vtn,(sz)*sizeof(pxgVertex3dTN),sz);
                    break;
                }
                case PXG_VERTEX3DN:
                {
                    pxgVertex3dN* vn;
                    vn = new pxgVertex3dN[objects[i]->faces.size()];
                    sz = FillVN(positions, normals,objects[i]->faces, vn);
                    o->SetVertexLayout(PXG_VERTEX3DN);
                    o->SetVertices(vn,sz*sizeof(pxgVertex3dN),sz);
                    break;
                }
                case PXG_VERTEX3DT:
                {
                    pxgVertex3dT* vt;
                    vt = new pxgVertex3dT[objects[i]->faces.size()];
                    sz = FillVT(positions,texcoords,objects[i]->faces, vt);
                    o->SetVertexLayout(PXG_VERTEX3DT);
                    o->SetVertices(vt,sz*sizeof(pxgVertex3dT),sz);
                    break;
                }
                case PXG_VERTEX3D:
                {
                    pxgVertex3d* v3d;
                    v3d = new pxgVertex3d[objects[i]->faces.size()];
                    sz = FillV3D(positions, objects[i]->faces, v3d);
                    obj->SetVertexLayout(PXG_VERTEX3D);
                    obj->SetVertices(v3d,sz*sizeof(pxgVertex3d),sz);
                    break;
                }
            } //switch block

            obj_material* mat = NULL;
            //PXG::Log(to_string(materials.size()));
            for(int j = 0;j<materials.size();j++)
            {
                if(materials[j]!=NULL)
                    if(materials[j]->name == objects[i]->matName)
                        {
                            mat = materials[j];
                            break;
                        }
            }
            if(mat!=NULL)
            {
                o->SetAmbient(mat->ambient);
                o->SetDiffuse(mat->diffuse);
                o->SetSpecular(mat->specular);
                if(mat->texture!=NULL)
                {
                    o->SetTexture(mat->texture,PXG_TEXTURE0);
                }
            }
            //obj->AddChildMesh(o);
           // o->SetFlags(PXG_USE_LIGHTING);
            //PXG::Log("child mesh");

        } //else block
    }
    return obj;

}

std::vector<pxgMeshLib::obj_material*> pxgMeshLib::LoadMTL(std::string fname)
{
    PXG::Log("pxgMeshLib: Loading OBJ material file: "+fname);

    using namespace std;

    int numMaterials = 0;

    vector<obj_material*> materials;
    materials.push_back(new obj_material);

    ifstream f(fname);
    vector<string> lines;
    while(f.good())
    {
        string l;
        getline(f,l);
        lines.push_back(l);
    }

    for(int i = 0;i<lines.size();i++)
    {
        switch(lines[i][0])
        {
            case '#':
                break;
            case 'n':
            {
                stringstream ss(lines[i]);
                string val;
                getline(ss,val,' ');
                if(val!="newmtl")
                {
                    PXG::Log("pxgMeshLib::LoadMTL() warning: unknown token: "+val,WARN);
                    break;
                }
                getline(ss,val);
                PXG::Log("pxgMeshLib::LoadMTL(): new material found: "+val);
                if(materials[numMaterials]->name.empty())
                {
                   // PXG::Log(val);
                    materials[numMaterials]->name = val;
                }
                else
                {
                    materials.push_back(new obj_material);
                    numMaterials++;
                    materials[numMaterials]->name = val;
                }

                break;
            }
            case 'K':
            {
                switch(lines[i][1])
                {
                    case 'a': //ambient
                    {
                        stringstream ss(lines[i]);
                        string val;
                        getline(ss,val,' ');
                        getline(ss,val,' ');
                        if(!val.empty())
                            materials[numMaterials]->ambient.x = stof(val);

                        getline(ss,val,' ');
                        if(!val.empty())
                            materials[numMaterials]->ambient.y = stof(val);

                        getline(ss,val,' ');
                        if(!val.empty())
                            materials[numMaterials]->ambient.z = stof(val);
                        break;
                    }
                    case 'd': //diffuse
                    {
                        stringstream ss(lines[i]);
                        string val;
                        getline(ss,val,' ');
                        getline(ss,val,' ');
                        if(!val.empty())
                            materials[numMaterials]->diffuse.x = stof(val);

                        getline(ss,val,' ');
                        if(!val.empty())
                            materials[numMaterials]->diffuse.y = stof(val);

                        getline(ss,val,' ');
                        if(!val.empty())
                            materials[numMaterials]->diffuse.z = stof(val);
                        break;
                    }
                    case 's': //specular
                    {
                        stringstream ss(lines[i]);
                        string val;
                        getline(ss,val,' ');
                        getline(ss,val,' ');
                        if(!val.empty())
                            materials[numMaterials]->specular.x = stof(val);

                        getline(ss,val,' ');
                        if(!val.empty())
                            materials[numMaterials]->specular.y = stof(val);

                        getline(ss,val,' ');
                        if(!val.empty())
                            materials[numMaterials]->specular.z = stof(val);

                        break;
                    }
                }
                break;
            }
            case 'm':
            {
                stringstream ss(lines[i]);
                string val;
                getline(ss,val,' ');
                PXG::Log(val);
                materials[numMaterials]->texture = NULL;
                if(strcmp(val.c_str(),"map_Kd")==0)
                {
                    getline(ss,val,' ');
                    if(FileExists(string(GetPath(fname)+val).c_str()))
                    {
                        pxgTexture* tex = pxgEngine::TexturePool()->QueryTexture(string(GetPath(fname)+val).c_str(),PXG_FILTER_MIPMAP_LINEAR|PXG_TEX_REPEAT_X|PXG_TEX_REPEAT_Y);
                        materials[numMaterials]->texture = tex;
                    }
                }
                break;
            }
        }

    }

    for(int i = 0;i<materials.size();i++)
    {
        PXG::Log("Material name: "+materials[i]->name+". Texture: "+(materials[i]->texture==NULL?"":materials[i]->texture->GetFname()));
        PXG::Log("Ambient: "+to_string(materials[i]->ambient.x)+" | "+to_string(materials[i]->ambient.y)+" | "+to_string(materials[i]->ambient.x));
        PXG::Log("Diffuse: "+to_string(materials[i]->diffuse.x)+" | "+to_string(materials[i]->diffuse.y)+" | "+to_string(materials[i]->diffuse.z));
        PXG::Log("Specular: "+to_string(materials[i]->specular.x)+" | "+to_string(materials[i]->specular.y)+" | "+to_string(materials[i]->specular.z));
    }

    return materials;

}

int pxgMeshLib::FillVTN(ArrayVec3 positions, ArrayVec2 texcoords, ArrayVec3 normals, ArrayIvec3 faces, pxgVertex3dTN* out)
{
    if(faces.size()<=0||positions.size()<=0||texcoords.size()<=0||normals.size()<=0)
        return 0;
    //out = new pxgVertex3dTN[faces.size()];

   // PXG::Log(std::to_string(positions.size()));
    //PXG::Log(std::to_string(texcoords.size()));
    //PXG::Log(std::to_string(normals.size()));

    for(int i = 0;i<faces.size();i++)
    {
        out[i].pos = positions[faces[i].x-1];
        out[i].tc = texcoords[faces[i].y-1];
        out[i].normal = normals[faces[i].z-1];
    }
    /*for(int i = 0;i<faces.size();i++)
        PXG::Log(std::to_string(out[i].pos.x)+" "+std::to_string(out[i].pos.y)+" "+std::to_string(out[i].pos.z)+
        " | "+std::to_string(out[i].tc.x)+" "+std::to_string(out[i].tc.y)+
        " | "+std::to_string(out[i].normal.x)+" "+std::to_string(out[i].normal.y)+" "+std::to_string(out[i].normal.z));*/
    return faces.size();
}


int pxgMeshLib::FillVN(ArrayVec3 positions, ArrayVec3 normals, ArrayIvec3 faces, pxgVertex3dN* out)
{
    if(faces.size()<=0||positions.size()<=0||normals.size()<=0)
        return 0;

   // PXG::Log(std::to_string(positions.size()));
    //PXG::Log(std::to_string(normals.size()));

    for(int i = 0;i<faces.size();i++)
    {
        out[i].pos = positions[faces[i].x-1];
        out[i].normal = normals[faces[i].z-1];
    }
    return faces.size();
}

int pxgMeshLib::FillVT(ArrayVec3 positions, ArrayVec2 texcoords, ArrayIvec3 faces, pxgVertex3dT* out)
{
    if(faces.size()<=0||positions.size()<=0||texcoords.size()<=0)
        return 0;

    //PXG::Log(std::to_string(positions.size()));
    //PXG::Log(std::to_string(texcoords.size()));

    for(int i = 0;i<faces.size();i++)
    {
        out[i].pos = positions[faces[i].x-1];
        out[i].tc = texcoords[faces[i].y-1];

    }
    return faces.size();
}

int pxgMeshLib::FillV3D(ArrayVec3 positions, ArrayIvec3 faces, pxgVertex3d* out)
{
    if(faces.size()<=0||positions.size()<=0)
        return 0;

    //PXG::Log(std::to_string(positions.size()));

    for(int i = 0;i<faces.size();i++)
    {
        out[i].pos = positions[faces[i].x-1];;
    }
    return faces.size();
}

pxgObject* pxgMeshLib::CreateSkybox()
{
    pxgObject* obj = new pxgObject();
    pxgVertex3d vertices[] = {
    glm::vec3(-1.0, -1.0,  1.0),
    glm::vec3(1.0, -1.0,  1.0),
    glm::vec3(-1.0,  1.0,  1.0),
    glm::vec3(1.0,  1.0,  1.0),
    glm::vec3(-1.0, -1.0, -1.0),
    glm::vec3(1.0, -1.0, -1.0),
    glm::vec3(-1.0,  1.0, -1.0),
    glm::vec3(1.0,  1.0, -1.0)
    };

    GLuint indices[] = {
    0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1
    };

    obj->Init();
    obj->SetVertexLayout(PXG_VERTEX3D);
    obj->SetVertices((void*)vertices,sizeof(vertices),sizeof(vertices)/sizeof(pxgVertex3d));
    obj->SetIndices((void*)indices,sizeof(indices),sizeof(indices)/sizeof(GLuint));
    return obj;
}

pxgObject* pxgMeshLib::CreateGridMesh(int resolution)
{

    pxgVertex3dT *verts = new pxgVertex3dT[(resolution)*(resolution)];
  /* pxgVertex3dT verts[] =
   {
        vec3(), vec2();
   };*/
    int count = 0;
    int width = resolution/2;

    for(int i = 0;i<resolution;i++)
        for(int j = 0;j<resolution;j++)
        {
            verts[count].pos = vec3(i-resolution/2,0,j-resolution/2);
            verts[count++].tc = vec2(float(i)/resolution,float(j)/resolution);
        }
   // PXG::Log(std::to_string(count));

    GLuint *indices = new GLuint[(resolution-1)*(resolution-1)*6];

   // PXG::Log(std::to_string(sizeof(indices)+sizeof(verts)));

    count = 0;
    for(int i = 0;i<resolution-1;i++)
    {
        for(int j = 0;j<resolution-1;j++)
        {
            indices[count++] = i*resolution+j;
            indices[count++] = i*resolution+j+1;
            indices[count++] = (i+1)*resolution+j;
            indices[count++] = i*resolution+j+1;
            indices[count++] = (i+1)*resolution+j+1;
            indices[count++] = (i+1)*resolution+j;
           // indices[count++] = i*resolution+j+1;
        }
    }
    PXG::Log(std::to_string(count));

   /* GLuint indices[] =
    {
       resolution, 0, 1, 1,
       resolution+1, resolution
    };*/

    pxgObject* obj = new pxgObject;
    obj->Init();
    obj->SetVertexLayout(PXG_VERTEX3DT);
    obj->SetVertices((void*)verts,sizeof(pxgVertex3dT)*(resolution)*(resolution),(resolution)*(resolution));
    obj->SetIndices((void*)indices,sizeof(GLuint)*(resolution-1)*(resolution-1)*6,(resolution-1)*(resolution-1)*6);
    return obj;
}

pxgObject* pxgMeshLib::Load3DS(std::string fname)
{
    using namespace std;
    ifstream f(fname,ios::binary);
    if(!f.is_open())
        return NULL;
    unsigned short chunk;
    unsigned int chunkLength;
    PXG::Log("asdasd");
    unsigned long sz = FileSize(fname);
    PXG::Log(std::to_string(sz));
   /* PXG::Log(std::to_string(f.tellg()));
    while(f.tellg()<100);
    {
       // f.read((char*)&chunk,2);
       // f.read((char*)&chunkLength,4);
        PXG::Log(std::to_string(f.tellg()));
    }*/
    return new pxgObject();
}

pxgObject* pxgMeshLib::CreateTestAnim()
{
    pxgObject* obj = new pxgObject;
    obj->Init();

    float width = 10;
    float height = 10;

    pxgVertex3dTAnim data[] =
    {
        { vec3(-width/2.f, 0,height/2.f),vec2(0.f,0.f), ivec4(100), vec4(1) },
        { vec3(width/2.f,  0,height/2.f),vec2(1.0f,0.f), ivec4(0), vec4(0)},
        { vec3(width/2.f, 0, -height/2.f),vec2(1.0f,1.0f), ivec4(100), vec4(0) },
        { vec3(-width/2.f,0,  -height/2.f),vec2(0.f,1.0f), ivec4(0), vec4(0)}
    };

    GLuint elements[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    obj->SetVertexLayout(PXG_VERTEX3DTANIM);
    obj->SetVertices(data,sizeof(data),sizeof(data)/sizeof(pxgVertex3dTAnim));
    obj->SetIndices(elements,sizeof(elements),sizeof(elements)/sizeof(GLuint));
    return obj;

}


bool pxgMeshLib::SaveObject(std::string fname, pxgObject* obj)
{
    if(!obj)
        return false;
    pxgTRIHeader hdr;
    hdr.ident[0] = 'P';
    hdr.ident[1] = 'X';
    hdr.ident[2] = 'G';
    hdr.ident[3] = 'T';
    hdr.ident[4] = 'R';
    hdr.ident[5] = 'I';
    hdr.version = 100;
  //  hdr.
}

#define OBJECT_CHUNK 0x10
#define OBJECT_TYPE_CHUNK 0x11
#define OBJECT_MESH 0x30
#define OBJECT_POINT_LIGHT 0x31
#define OBJECT_DIR_LIGHT 0x32
#define OBJECT_SPOT_LIGHT 0x33
#define OBJECT_LOGIC 0x34
#define MATERIAL_CHUNK 0x12
#define ANIMATION_CHUNK 0x13
#define ANIMATION_KEY 0x14
#define TRANSFORMATION_CHUNK 0x15
#define MESH_DATA 0x17
#define VERTEX_LAYOUT 0x18
#define VERTEX_DATA_START 0x19
#define VERTEX_DATA_END 0x20
#define END_CHUNK 0x16
#define MESH_DATA_END 0x21

pxgSceneFile* pxgMeshLib::LoadPxgScene(std::string fname)
{
    if(!FileExists(fname))
    {
        PXG::Log("pxgMeshLib::LoadPxgScene(): file does not exist: "+fname, ERR);
        return NULL;
    }

    using namespace std;
    using namespace glm;

    fstream file(fname.c_str(),std::ios::binary|std::ios::in);
    char ident[7];
    file.read(ident,7);
    if(strcmp(ident,"PXGSCN")!=0)
    {
        PXG::Log("pxgMeshLib::LoadPxgScene(): file "+fname+" is not valip PXGSCN file", ERR);
        return NULL;
    }

    vector<pxgSceneFileObject*> objects;

    char chunk;
    while(file.read(&chunk,1))
    {
        switch(chunk)
        {
            case OBJECT_CHUNK:
            {
                pxgSceneFileObject* o = new pxgSceneFileObject;
                o->type = PXG_OBJECT_UNKNOWN;
                objects.push_back(o);
                string objName;
                char c;
                do
                {
                    file.read(&c,1);
                    if(c)
                        objName.push_back(c);
                }while(c!='\0');
                PXG::Log(objName);

                c = 0;
                while(c!=END_CHUNK)
                {
                    file.read(&c,1);
                    //
                    PXG::Log(to_string(c));
                    switch(c)
                    {
                        case OBJECT_TYPE_CHUNK:
                        {
                            file.read(&c,1);
                            switch(c)
                            {
                                case OBJECT_MESH:
                                    objects[objects.size()-1]->ptr = new pxgObject();
                                    break;
                                case OBJECT_LOGIC:
                                    objects[objects.size()-1]->ptr = new pxgLogicalObject();
                                    break;
                                case OBJECT_DIR_LIGHT:
                                    objects[objects.size()-1]->ptr = new pxgLight(PXG_LIGHT_DIRECTIONAL);
                                    break;
                                case OBJECT_POINT_LIGHT:
                                    objects[objects.size()-1]->ptr = new pxgLight(PXG_LIGHT_POINT);
                                    break;
                                case OBJECT_SPOT_LIGHT:
                                    objects[objects.size()-1]->ptr = new pxgLight(PXG_LIGHT_SPOT);
                                    break;
                            }
                            break;
                        }
                        case MATERIAL_CHUNK:
                        {
                            pxgMaterial* mat = new pxgMaterial();
                            vec3 a;
                            file.read((char*)value_ptr(a),sizeof(vec3));
                            Print(a);
                            mat->Ambient(a);
                            file.read((char*)value_ptr(a),sizeof(vec3));
                            Print(a);
                            mat->Diffuse(a);
                            file.read((char*)value_ptr(a),sizeof(vec3));
                            Print(a);
                            mat->Specular(a);
                            float *f = new float;
                            file.read((char*)f,sizeof(float));
                            PXG::Log(to_string(*f));
                            mat->Shininess(*f);
                            file.read((char*)f,sizeof(float));
                            PXG::Log(to_string(*f));

                            char tx;
                            string texName;
                            do
                            {
                                file.read(&tx,1);
                                if(tx)
                                    texName.push_back(tx);
                            }while(tx);

                            if(!texName.empty())
                                PXG::Log(texName);

                            break;
                        }

                        case ANIMATION_CHUNK:
                        {
                            int *numFrames = new int;
                            file.read((char*)numFrames,sizeof(int));
                            PXG::Log(to_string(*numFrames));
                            file.seekg(file.tellg()+sizeof(float)*9*(*numFrames));
                            break;
                        }
                        case TRANSFORMATION_CHUNK:
                        {
                            vec3 p;
                            file.read((char*)value_ptr(p),sizeof(vec3));
                            Print(p);
                            vec3 r;
                            file.read((char*)value_ptr(r),sizeof(vec3));
                            Print(r);
                            vec3 s;
                            file.read((char*)value_ptr(s),sizeof(vec3));
                            Print(p);
                            break;
                        }
                        case MESH_DATA:
                        {
                        pxgObject* obj = (pxgObject*)objects[objects.size()-1]->ptr;
                        obj->Init();
                            do
                            {
                                file.read(&c,1);
                                switch(c)
                                {
                                    case VERTEX_LAYOUT:
                                    {
                                        file.read(&c,1);
                                        switch(c)
                                        {
                                            case 1:
                                            obj->SetVertexLayout(PXG_VERTEX3D);
                                            break;
                                            case 2:
                                            obj->SetVertexLayout(PXG_VERTEX3DT);
                                            break;
                                            case 3:
                                            obj->SetVertexLayout(PXG_VERTEX3DTN);
                                            break;
                                        }
                                        break;
                                    }
                                    case VERTEX_DATA_START:
                                    {
                                        int *numVerts = new int;
                                        file.read((char*)numVerts,sizeof(int));
                                        PXG::Log(to_string(*numVerts));
                                        if(!*numVerts)
                                            break;
                                        pxgVertex3d* verts = new pxgVertex3d[*numVerts];
                                        file.read((char*)verts,sizeof(pxgVertex3d)*(*numVerts));
                                        break;
                                    }
                                }
                            }while(c!=MESH_DATA_END);
                            break;
                        }

                    }
                }
                break;
            }
        }
    }

}

