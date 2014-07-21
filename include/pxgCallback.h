/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXGCALLBACK_H
#define PXGCALLBACK_H

template<typename _ret_T, typename _param_T, typename _container_T>
class pxgCallback
{
public:
    pxgCallback(_container_T *v): m_value(v) {}
    virtual _ret_T function(_param_T);
protected:
    _container_T *m_value;
};

#endif // PXGCALLBACK_H
