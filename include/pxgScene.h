/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXG_SCENE_H
#define PXG_SCENE_H

#include <pxgNode.h>


#include <vector>
#include <map>
#include <glm/glm.hpp>

class pxgCamera;
class pxgObject;
class pxgLight;
class pxgWorld;
class pxgInstanceGroup;


struct __pxg_light
{
    __pxg_light(pxgLight *light, bool e = true): l(light), enabled(e) {}
    pxgLight* l;
    bool enabled;
};

struct __pxg_node
{
    __pxg_node(pxgNode* node, bool h = false): n(node), hidden(h){}
    pxgNode* n;
    bool hidden;
};

class pxgScene: public pxgNode
{
public:
    pxgScene();
	bool Init();
	void AddObject(pxgObject*);
	void AddInstance(pxgObject*, glm::mat4 transform);
	void SetCamera(pxgCamera *pCamera) { camera = pCamera; }
	void RemoveObject(pxgObject*);
	void HideObject(pxgObject*);
	void Update();
	void SetAmbient(glm::vec4 a) { ambient = a; }

    void DrawForPicking();
    pxgObject* GetObjectByID(int id);

    void AddInstanceGroup(pxgInstanceGroup*);


	void AddLight( pxgLight *light);
	void RemoveLight(pxgLight *light);
	void DisableLight(pxgLight *light);
	void EnableLight(pxgLight *light);

	void EnableLighting() { lighting = true; }
	void DisableLighting() { lighting = false; }

	size_t GetLightCount() const { return lights.size(); }
	size_t GetLightCount(pxgObject* obj) const;

    std::vector<pxgLight*> GetLights(pxgObject *obj) const;

	const glm::vec4 GetAmbient() const { return ambient; }
	const glm::mat4 GetView() const;
	const glm::mat4 GetProj() const;
	const glm::mat4 GetVP() const;

	pxgWorld* GetWorld() const { return world; }
	void SetWorld(pxgWorld* w) { world = w; }

	bool Render();
	void Destroy();
private:
    std::vector<pxgObject*> objects;
	std::vector<__pxg_light*> lights;

    pxgWorld* world;

    std::vector<pxgInstanceGroup*> instanceGroups;

    typedef std::map<pxgObject*,std::vector<glm::mat4> > InstanceList;
    InstanceList instances;

    void DrawInstances();

	pxgCamera *camera;
	glm::vec4 ambient;

	bool lighting;
};

#endif //PXG_SCENE_H
