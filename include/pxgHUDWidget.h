/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXGHUDWIDGET_H
#define PXGHUDWIDGET_H

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
};

#endif // PXGHUDWIDGET_H
