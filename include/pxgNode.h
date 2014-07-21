/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXG_NODE_H
#define PXG_NODE_H

#include <vector>
#include <string>

#include <glm/glm.hpp>

class pxgNode
{
public:
	pxgNode();
	//pxgNode(const pxgNode& other);
	virtual ~pxgNode() { }
	virtual void Update() = 0;
	virtual bool Render() = 0;
	virtual void Destroy() = 0;

	//virtual pxgNode* Copy(pxgNode *newNode);

	void UpdateTransformation();

	void SetParent(pxgNode* __parent) {parent = __parent;}
	void AddChild(pxgNode* n);
	size_t GetNodeCount() const;

	void DeleteChild(pxgNode* n);

	void Rotate(glm::vec3 rotationXYZ) { rotation = rotationXYZ; }
	glm::vec3 GetRotation() const { return rotation; }
	void Scale(glm::vec3 scalingXYZ) { scaling = scalingXYZ; }
	glm::vec3 GetScaling() const { return scaling; }
	void Translate(glm::vec3 translationXYZ) { translation = translationXYZ; }
	glm::vec3 GetTranslation() const { return translation; }
	glm::mat4 GetTransformation() { return transformation; }
	void Transform(glm::mat4 t) { m_transform = t; }

    std::string GetName() const { return name; }
    void SetName(std::string n) { name = n; }
protected:
	pxgNode *parent;
	std::vector<pxgNode*> childs;

    glm::mat4 transformation, m_transform;
	glm::vec3 scaling, rotation, translation;

    std::string name;

};

#endif //PXG_NODE_H
