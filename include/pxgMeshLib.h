/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXGPRIMITIVE
#define PXGPRIMITIVE

class pxgObject;
class pxgShader;
class pxgTexture;
class pxgLight;
class pxgNode;
class pxgLogicalObject;

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include <pxgVertexLayouts.h>

enum PXG_MODEL_TYPE
{
    PXG_WAVEFRONT_OBJ,
    PXG_3DS
};

struct pxgTRIHeader
{
    char ident[6]; //"PXGTRI"
    int32_t version; //100
    int32_t vertexCount;
    int32_t vertexLayout;
};

enum pxgSceneFileObjectType
{
    PXG_OBJECT_UNKNOWN,
    PXG_OBJECT_MESH,
    PXG_OBJECT_LIGHT,
    PXG_OBJECT_LOGICAL
};

struct pxgSceneFileObject
{
    pxgSceneFileObjectType type;
    void* ptr;
};

struct pxgSceneFile
{
    std::string name;
    std::vector<pxgNode*> objects;
};

class pxgMeshLib
{
public:
    void Init();
    pxgObject* GetUnitCubeWithNormals();
    pxgObject* GetUnitCube();

    pxgObject* CreateSkybox();

    pxgObject* GetUnitPlaneWithNormals();
    pxgObject* GetUnitPlane();

    pxgObject* CreateCubeMesh(float width, float height, float depth);
    pxgObject* CreatePlaneMesh(float width, float height);

    pxgObject* CreatePlaneMeshWithNormals(float width, float height);
    pxgObject* CreateCubeMeshWithNormals(float width, float height, float depth);

    pxgObject* LoadFromFile(std::string fname, PXG_MODEL_TYPE type);

    pxgObject* CreateGridMesh(int resolution);

    pxgObject* CreateTestAnim();

    bool SaveObject(std::string fname, pxgObject *obj);
    pxgSceneFile* LoadPxgScene(std::string fname);

private:
    pxgObject* m_cbMesh;
    pxgObject* m_cbnMesh;
    pxgObject* m_plMesh;
    pxgObject* m_plnMesh;

    pxgObject* LoadObj(std::string fname);
    pxgObject* LoadObjMulti(std::string fname);

    pxgObject* Load3DS(std::string fname);

    struct obj_material
    {
        obj_material():texture(NULL) {}
        std::string name;
        pxgTexture* texture;
        glm::vec3 diffuse;
        glm::vec3 ambient;
        glm::vec3 specular;
    };


    std::vector<obj_material*> LoadMTL(std::string fname);

    typedef std::vector<glm::vec3> ArrayVec3;
    typedef std::vector<glm::vec2> ArrayVec2;
    typedef std::vector<glm::ivec3> ArrayIvec3;
    int FillVTN(ArrayVec3 positions, ArrayVec2 texcoords, ArrayVec3 normals, ArrayIvec3 faces, pxgVertex3dTN* out);
    int FillVT(ArrayVec3 positions, ArrayVec2 texcoords, ArrayIvec3 faces, pxgVertex3dT* out);
    int FillVN(ArrayVec3 positions, ArrayVec3 normals, ArrayIvec3 faces, pxgVertex3dN* out);
    int FillV3D(ArrayVec3 positions, ArrayIvec3 faces, pxgVertex3d* out);
};

/*pxgObject* pxgCreateCubeMesh(float width, float height, float depth);
pxgObject* pxgCreatePlaneMesh(float width, float height);

pxgObject* pxgCreatePlaneMeshWithNormal(float width, float height);
pxgObject* pxgCreateCubeMeshWithNormal(float width, float height, float depth);*/


#endif //PXGPRIMITIVE
