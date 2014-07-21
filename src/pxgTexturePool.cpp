/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgTexturePool.h>
#include <algorithm>

#include <pxgTexture.h>
#include <PXG.h>

pxgTexturePool::pxgTexturePool()
{}

pxgTexture* pxgTexturePool::QueryTexture(std::string fname, int flags)
{
    if(textures.find(fname)!=textures.end())
        return textures[fname]->t;
    else
    {
        pxgTexture* tex = new pxgTexture();
        __tex* t = new __tex;
        t->t = tex;
        t->numUses = 1;
        tex->LoadTexture(fname, flags);
        textures[fname] = t;
        return tex;
    }
}

void pxgTexturePool::RemoveTexture(std::string name)
{
    if(textures.find(name)!=textures.end())
    {
        textures[name]->numUses--;
    }
}

void pxgTexturePool::RemoveGarbage()
{
    for(texList::iterator i = textures.begin();i!=textures.end();i++)
    {
        __tex* t = i->second;
        if(!t->numUses)
            DeleteTexture(i->first);
    }
}

void pxgTexturePool::DeleteTexture(std::string name)
{
    if(textures.find(name)!=textures.end())
    {
        delete textures[name];
        textures.erase(name);
    }
}
