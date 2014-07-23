/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXGHUDWIDGET_H
#define PXGHUDWIDGET_H

#include <vector>

class pxgHUDManager;
class pxgHUDObject;

class pxgHUDWidget
{
public:
    pxgHUDWidget(pxgHUDManager*);
    void Render();
    void AddObject(pxgHUDObject* o);
    void RemoveObject(pxgHUDObject* o);
    pxgHUDObject* GetHoveredObject();
    pxgHUDObject* GetClickedObject();

private:
    pxgHUDManager* hudMan;
    std::vector<pxgHUDObject*> objects;
};

#endif // PXGHUDWIDGET_H
