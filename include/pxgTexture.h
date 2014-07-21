/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXGTEXTURE
#define PXGTEXTURE

typedef unsigned int GLuint;
typedef int GLint;

#include <vector>
#include <string>

enum PXGTEXTURE_FLAGS
{
    PXG_FILTER_NEAREST = 0,
    PXG_FILTER_LINEAR = 1<<1,
    PXG_FILTER_MIPMAP_NEAREST = 1<<2,
    PXG_FILTER_MIPMAP_LINEAR = 1<<3,
    PXG_TEX_REPEAT_X = 1<<4,
    PXG_TEX_REPEAT_Y = 1<<5,
    PXG_TEX_REPEAT_Z = 1<<6,
    PXG_TEX_CLAMP_X = 1<<7,
    PXG_TEX_CLAMP_Y = 1<<8,
    PXG_TEX_CLAMP_Z = 1<< 9,
    PXG_TEX_EDGECLAMP_X = 1<<10,
    PXG_TEX_EDGECLAMP_Y = 1<<11,
    PXG_TEX_EDGECLAMP_Z = 1<<12,
    PXG_TEX_BORDERCLAMP_X = 1<<13,
    PXG_TEX_BORDERCLAMP_Y = 1<<14,
    PXG_TEX_BORDERCLAMP_Z = 1<<15
};

enum PXGTEXTURE_TYPE
{
    PXG_TEXTURE_2D,
    PXG_TEXTURE_CUBEMAP,
    PXG_TEXTURE_ARRAY
};

enum PXG_TEXTURE_STAGE
{
    PXG_TEXTURE0 = 0,
    PXG_TEXTURE1,
    PXG_TEXTURE2,
    PXG_TEXTURE3,
    PXG_TEXTURE4,
    PXG_TEXTURE5,
    PXG_TEXTURE6,
    PXG_TEXTURE7
};

enum PXG_TEXTURE_FORMAT
{
    PXG_TEXTURE_RGB_UB,
    PXG_TEXTURE_RGBA_UB,
    PXG_TEXTURE_RGB_US,
    PXG_TEXTURE_RGBA_US,
    PXG_TEXTURE_R_UB,
    PXG_TEXTURE_R_US
};

class pxgTexture
{
public:
    pxgTexture();
    ~pxgTexture();
    bool LoadTexture(std::string fname, int flags = PXG_TEX_REPEAT_X|PXG_TEX_REPEAT_Y|PXG_FILTER_MIPMAP_LINEAR);
    bool LoadHeightmap(std::string fname, int flags);
	bool LoadRawHeightmap(std::string fname, int flags, int width, int height);
    bool LoadArray(std::vector<std::string> fnames, int flags = PXG_TEX_REPEAT_X|PXG_TEX_REPEAT_Y|PXG_FILTER_MIPMAP_LINEAR);
	bool LoadCubemap(std::string baseName, int flags);
    bool CreateEmptyTexture(int width, int height, PXG_TEXTURE_FORMAT format);
	void Delete();
	GLuint GetName();
	std::string GetFname() { return m_fname; }
	GLuint GetTexUnit() { return texUnit; }
    bool Use(GLuint texUnit);
    void Unuse();
private:
    GLuint name;
    std::string m_fname;
    GLuint texUnit;
    int type;
    int m_flags;
};

#endif //PXGTEXTURE
