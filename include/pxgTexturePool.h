/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXGTEXTUREPOOL_H
#define PXGTEXTUREPOOL_H

#include <map>
#include <string>

#include <pxgTexture.h>

class pxgTexture;

class pxgTexturePool
{
public:
    pxgTexturePool();
    pxgTexture* QueryTexture(std::string fname, int flags = PXG_TEX_REPEAT_X|PXG_TEX_REPEAT_Y|PXG_FILTER_MIPMAP_LINEAR);
    void RemoveTexture(std::string name);
    void RemoveGarbage();
    void DeleteTexture(std::string name);

private:


    struct __tex
    {
        pxgTexture* t;
        unsigned short numUses;
    };

    typedef std::map<std::string, __tex*> texList;
    texList textures;
};

#endif // PXGTEXTUREPOOL_H
