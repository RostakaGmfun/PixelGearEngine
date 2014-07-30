/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXGOBJECT_H
#define PXGOBJECT_H

class pxgCamera;
class pxgScene;
class pxgBuffer;
class pxgShader;

#include <glm/fwd.hpp>
#include <map>
#include <vector>

#include <pxgNode.h>
#include <pxgTexture.h>
#include <pxgVertexLayouts.h>
#include <pxgMaterial.h>

enum PXGOBJECT_FLAGS
{
    PXG_USE_AMBIENT = 1<<1,
    PXG_USE_LIGHTING = 1<<2,
    PXG_DRAW_WIREFRAME = 1<<3,
    PXG_DRAW_BILLBOARD = 1<<4,
    PXG_ENABLE_DYNAMICS = 1<<5,
    PXG_ENABLE_PICKING = 1<<6
};

struct pxgSkeleton
{

};

class pxgScene;
class pxgRigidBody;
class pxgWorld;

class pxgObject: public pxgNode
{
public:
    pxgObject();
    ~pxgObject();
	bool Init();
	bool InitBillboard(float width, float height);
	void BillboardTexture(pxgTexture*);
	void BillboardColor(glm::vec4);
	void SetVertexBuffer(pxgBuffer *vb);
	bool InitVBO();
	bool InitIBO();
    pxgBuffer* GetVBO() const { return vbo; }
    pxgBuffer* GetIBO() const { return ibo; }
	void SetVertices(void* data, int sz, int count);
    int GetVertexLayout() const { return layout; }
	void SetIndices(void* data, int sz, int count);
	void SetVertexLayout(PXG_VERTEX_LAYOUT layout);
    void SetShader(pxgShader* _shader);
    pxgShader* GetShader() const { return shader; }
    void SetVisible(bool e){ visible = e; }
    bool IsVisible() const { return visible; }

    void BeginRenderInstance();
    void EndRenderInstance();

    void SetMaterial(pxgMaterial m) { mat = m; }
    pxgMaterial GetMaterial() const { return mat;}

    std::vector<pxgObject*> GetChildMeshes() const { return meshList; }
    pxgScene* GetScene() const { return scene; }

	void SetTexture(pxgTexture* tex, PXG_TEXTURE_STAGE stage);

	void SetAmbient(glm::vec3 a) { ambient = a; }
	glm::vec3 GetAmbient() const { return ambient; }
	void SetDiffuse(glm::vec3 d) { diffuse = d; }
	glm::vec3 GetDiffuse() const { return diffuse; }
	void SetSpecular(glm::vec3 s) { specular = s; }
	glm::vec3 GetSpecular() { return specular; }

	void SetScene(pxgScene*  pScene);
	void SetWorld(pxgWorld* world);


	void SetFlags(int flag);
	int GetFlags() const { return flags; }

	void DeleteTexture(std::string name);

	void AddChildMesh(pxgObject* obj);
	void RemoveChildsMesh(pxgObject* obj);

    void RenderChildMeshes();
    void UpdateChildMeshes();

	void Update();
	bool Render();
    void Destroy();

	//physics
    void SetRigidBody(pxgRigidBody* rb) { rBody  = rb; }
    pxgRigidBody* GetRigidBody() const { return rBody; }
    void ApplyForce(glm::vec3 f);
    void AddImpulse(glm::vec3 i);


private:

    typedef std::vector<pxgObject*> MeshList;
	MeshList meshList;

	typedef std::map<PXG_TEXTURE_STAGE, pxgTexture*> texObj;
	texObj textures;

    bool DrawBillboard();
    bool DrawPicking();
    void SetupMaterial();

    void UpdateFromRigidBody();

	pxgBuffer *vbo, *ibo;
	pxgTexture *texture;
	pxgShader *shader;

	GLuint vao;
	GLuint bbVAO;
	pxgBuffer* bbVB;
	pxgBuffer* bbIB;
	glm::vec2 bbSize;
	pxgTexture* bbTex;
	glm::vec4 bbColor;

	pxgScene *scene;
	pxgWorld *world;

	pxgRigidBody* rBody;

	//material properties
	glm::vec3 ambient, specular, diffuse;

    pxgMaterial mat;

    PXG_VERTEX_LAYOUT layout;

    glm::vec3 pickColor;

	int flags;

    bool visible;
};

#endif //PXGOBJECT_H
