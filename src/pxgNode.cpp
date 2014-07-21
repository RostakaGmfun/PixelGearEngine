/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgNode.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

pxgNode::pxgNode():parent(NULL)
{
    transformation = mat4(1);
    translation = rotation = vec3(0);
    scaling = vec3(1);
}

/*pxgNode* pxgNode::Copy(pxgNode* newNode)
{
    if(newNode==NULL)
        return NULL;
    newNode->childs.clear();
    for(int i = 0;i<childs.size();i++)
    {
        newNode->childs.push_back(childs[i]->Copy(NULL));
    }
    newNode->m_transform = m_transform;
    newNode->parent = parent;
    newNode->transformation = transformation;
    newNode->scaling = scaling;
    newNode->rotation = rotation;
    newNode->translation = translation;
    return newNode;
}*/

void pxgNode::AddChild(pxgNode* n)
{
    if(n==NULL)
        return;
    childs.push_back(n);
    return;
}

void pxgNode::DeleteChild(pxgNode* n)
{
    if(n == NULL)
        return;
    for(std::vector<pxgNode*>::iterator i = childs.begin();i!=childs.end();i++)
    {
        pxgNode* nn = *i;
        if(nn==n)
        {
            n->SetParent(NULL);
            childs.erase(i);
            return;
        }
    }
    return;
}

size_t pxgNode::GetNodeCount() const
{
    size_t c = 0;
    for(int i = 0;i<childs.size();i++)
    {
        c += childs[i]->GetNodeCount();
    }
    c++; //plus this item
    return c;
}

/*void pxgNode::Destroy()
{
	for(int i = 0;i<childs.size();i++)
		childs[i]->Destroy();
}*/

/*bool pxgNode::Render()
{
    for(int i = 0;i<childs.size();i++)
    {
        childs[i]->Render();
    }
    return true;
}*/
void pxgNode::UpdateTransformation()
{
    transformation = m_transform;
    transformation = glm::translate(transformation, translation);
    transformation = glm::rotate(transformation, rotation.y, vec3(0,1,0));
    transformation = glm::rotate(transformation, rotation.x, vec3(1,0,0));
    transformation = glm::rotate(transformation, rotation.z, vec3(0,0,1));
    transformation = glm::scale(transformation, scaling);

}
