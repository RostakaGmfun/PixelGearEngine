/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXGLOGICALOBJECT_H
#define PXGLOGICALOBJECT_H

#include <pxgNode.h>

class pxgLogicalObject:public pxgNode
{
public:
    void Destroy() {}
    void Update()  {}
    bool Render() {}
};

#endif // PXGLOGICALOBJECT_H
