/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgTexture.h>
#include <../SOIL.h>
#include <pxgTools.h>

#include <fstream>

pxgTexture::pxgTexture():name(0), texUnit(0)
{}

pxgTexture::~pxgTexture()
{
Delete();
}

bool pxgTexture::LoadTexture(std::string fname, int flags)
{
    type = PXG_TEXTURE_2D;
    glGenTextures(1, &name);
    int width, height;
    m_fname = fname;
    if(name==0)
    {
        PXG::Log("pxgTexture::LoadTexture(): error generating texture name",ERR);
        return false;
    }

    if(!FileExists(fname))
    {
        PXG::Log("pxgTexture::LoadTexture(): file does not exist: "+std::string(fname),ERR);
        return false;
    }

    unsigned char* image = SOIL_load_image(fname.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

    glBindTexture(GL_TEXTURE_2D, name);
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    if(flags & PXG_TEX_CLAMP_X)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
    }

    if(flags & PXG_TEX_CLAMP_Y)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);

    }

    if(flags & PXG_TEX_EDGECLAMP_X)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    }

    if(flags & PXG_TEX_EDGECLAMP_Y)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    }

    if(flags & PXG_TEX_REPEAT_X)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);

    }

    if(flags & PXG_TEX_REPEAT_Y)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

    }

    if(flags & PXG_TEX_BORDERCLAMP_X)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

    }

    if(flags & PXG_TEX_BORDERCLAMP_Y)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    }

    if(flags & PXG_FILTER_NEAREST)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    }

    if(flags & PXG_FILTER_LINEAR)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }

    if(flags & PXG_FILTER_MIPMAP_NEAREST || flags & PXG_FILTER_MIPMAP_LINEAR)
    {
        PXG::glGenerateMipmap(GL_TEXTURE_2D);

        if(flags & PXG_FILTER_MIPMAP_NEAREST)
        {
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        }

        if(flags & PXG_FILTER_MIPMAP_LINEAR)
        {
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }
    }

    glBindTexture(GL_TEXTURE_2D,NULL);

	PXG::Log("pxgTexture::LoadTexture(): loaded texture file: " + std::string(fname) + ". OpenGL name: "+std::to_string(name) +
	". Dimensions: "+std::to_string(width)+"x"+std::to_string(height),INFO);
	return true;
}

bool pxgTexture::LoadCubemap(std::string baseName, int flags)
{
    type = PXG_TEXTURE_CUBEMAP;
    glGenTextures(1, &name);
    int width, height;
    m_fname = baseName;
    std::string names[] =
    {
        "PX",
        "NX",
        "PZ",
        "NZ",
        "PY",
        "NY"
    };
    glBindTexture(GL_TEXTURE_CUBE_MAP,name);
    for(int i = 0;i<6;i++)
    {
        if(!FileExists(GetPath(baseName)+GetFileName(baseName)+names[i]+"."+GetExtension(baseName)))
        {
            PXG::Log("pxgTexture::LoadCubemap() error: file does not exist: "+std::string(GetPath(baseName)+GetFileName(baseName)+names[i]+"."+GetExtension(baseName)),ERR);
            return false;
        }

        unsigned char* image = SOIL_load_image(std::string(GetPath(baseName)+GetFileName(baseName)+names[i]+"."+GetExtension(baseName)).c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
        SOIL_free_image_data(image);
        PXG::Log("pxgTexture::LoadCubemap(): loaded cubemap face: " + GetPath(baseName)+GetFileName(baseName)+names[i]+"."+GetExtension(baseName) + ". OpenGL name: "+std::to_string(name) +
        ". Dimensions: "+std::to_string(width)+"x"+std::to_string(height),INFO);
    }

    if(flags & PXG_TEX_CLAMP_X)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S, GL_CLAMP);
    }

    if(flags & PXG_TEX_CLAMP_Y)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T, GL_CLAMP);

    }
    if(flags & PXG_TEX_CLAMP_Z)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R, GL_CLAMP);

    }

    if(flags & PXG_TEX_EDGECLAMP_X)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    }

    if(flags & PXG_TEX_EDGECLAMP_Y)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    }

    if(flags & PXG_TEX_EDGECLAMP_Z)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    }

    if(flags & PXG_TEX_REPEAT_X)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S, GL_REPEAT);

    }

    if(flags & PXG_TEX_REPEAT_Y)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T, GL_REPEAT);

    }

    if(flags & PXG_TEX_REPEAT_Z)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R, GL_REPEAT);

    }

    if(flags & PXG_TEX_BORDERCLAMP_X)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

    }

    if(flags & PXG_TEX_BORDERCLAMP_Y)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    }

    if(flags & PXG_TEX_BORDERCLAMP_Z)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

    }

    if(flags & PXG_FILTER_NEAREST)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    }

    if(flags & PXG_FILTER_LINEAR)
    {
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }

    if(flags & PXG_FILTER_MIPMAP_NEAREST || flags & PXG_FILTER_MIPMAP_LINEAR)
    {
        PXG::glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        if(flags & PXG_FILTER_MIPMAP_NEAREST)
        {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        }

        if(flags & PXG_FILTER_MIPMAP_LINEAR)
        {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP,NULL);
    return true;
}

bool pxgTexture::LoadArray(std::vector<std::string> fnames, int flags)
{
    if(fnames.size()<=0)
        return false;
    type = PXG_TEXTURE_ARRAY;
    glGenTextures(1,&name);
    if(!name)
    {
        PXG::Log("pxgTexture::LoadArray(): error generating texture name");
        return false;
    }

    std::vector<unsigned char*> images;
    unsigned char* img;

    int width = -1, height = -1;

    for(int i = 0;i<fnames.size();i++)
    {
        if(!FileExists(fnames[i]))
        {
            PXG::Log("pxgTexture::LoadArray(): file does not exist: "+fnames[i],ERR);
            return false;
        }
        int w,h;
        unsigned char* image = SOIL_load_image(fnames[i].c_str(),&w,&h,0,SOIL_LOAD_RGBA);
        img = image;
        if(width==-1&&height==-1)
        {
            width = w;

            height = h;
        }
        else
        if(w!=width||h!=height)
        {
            PXG::Log("pxgTexture::LoadArray(): size of texture \""+fnames[i]+"\" is not equal to the size of first texture. Width: "+std::to_string(width)+". Height: "+std::to_string(height),ERR);
            return false;
        }
        images.push_back(image);

    }
    glBindTexture(GL_TEXTURE_2D_ARRAY,name);
   // PXG::glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, fnames.size());
    glTexImage3D(GL_TEXTURE_2D_ARRAY,0,GL_RGB,width,height,fnames.size(),0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
    for(int i = 0;i<images.size();i++)
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY,0,0,0,i,width,height,1,GL_RGBA,GL_UNSIGNED_BYTE,images[i]);
    PXG::glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_REPEAT);
    PXG::Log("pxgTexture::LoadArray(): loaded "+std::to_string(fnames.size())+" textures with dimensions "+std::to_string(width)+"x"+std::to_string(height));
    glBindTexture(GL_TEXTURE_2D_ARRAY,NULL);
    return true;
}

bool pxgTexture::LoadHeightmap(std::string fname,int flags)
{
    type = PXG_TEXTURE_2D;
    glGenTextures(1, &name);
    int width, height;
    m_fname = fname;
    if(name==0)
    {
        PXG::Log("pxgTexture::LoadHeightmap(): error generating texture name",ERR);
        return false;
    }

    if(!FileExists(fname))
    {
        PXG::Log("pxgTexture::LoadHeightmap(): file does not exist: "+std::string(fname),ERR);
        return false;
    }

    unsigned char* image = SOIL_load_image(fname.c_str(), &width, &height, 0, SOIL_LOAD_AUTO);

    glBindTexture(GL_TEXTURE_2D, name);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    if(flags & PXG_TEX_CLAMP_X)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
    }

    if(flags & PXG_TEX_CLAMP_Y)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);

    }

    if(flags & PXG_TEX_EDGECLAMP_X)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    }

    if(flags & PXG_TEX_EDGECLAMP_Y)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    }

    if(flags & PXG_TEX_REPEAT_X)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);

    }

    if(flags & PXG_TEX_REPEAT_Y)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

    }

    if(flags & PXG_TEX_BORDERCLAMP_X)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

    }

    if(flags & PXG_TEX_BORDERCLAMP_Y)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    }

    if(flags & PXG_FILTER_NEAREST)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    }

    if(flags & PXG_FILTER_LINEAR)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }

    if(flags & PXG_FILTER_MIPMAP_NEAREST || flags & PXG_FILTER_MIPMAP_LINEAR)
    {
        PXG::glGenerateMipmap(GL_TEXTURE_2D);
        if(flags & PXG_FILTER_MIPMAP_NEAREST)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        }

        if(flags & PXG_FILTER_MIPMAP_LINEAR)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }
    }

    glBindTexture(GL_TEXTURE_2D,NULL);

	PXG::Log("pxgTexture::LoadHeightmap(): loaded heightmap: " + std::string(fname) + ". OpenGL name: "+std::to_string(name) +
	". Dimensions: "+std::to_string(width)+"x"+std::to_string(height),INFO);
	return true;
}

bool pxgTexture::LoadRawHeightmap(std::string fname, int flags, int width, int height)
{
    if(!FileExists(fname))
    {
        PXG::Log("pxgTexture::LoadRawHeightmap(): file not found: "+fname, ERR);
        return false;
    }
    glGenTextures(1,&name);
    glBindTexture(GL_TEXTURE_2D, name);

    using namespace std;
    ifstream f(fname,ios::binary);
    int s = f.tellg();
    f.seekg(0,ios::end);
    int sz = f.tellg()-s;
    if(sz<width*height)
    {
        PXG::Log("pxgTexture::LoadRawHeightmap() error: width and height does not match the size of data");
        return false;
    }
    GLushort *data = new GLushort[sz];
    f.seekg(0,ios::beg);
    f.read((char*)data,sz);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, width, height, 0, GL_RED, GL_UNSIGNED_SHORT, data);
    delete [] data;
     if(flags & PXG_TEX_CLAMP_X)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
    }

    if(flags & PXG_TEX_CLAMP_Y)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);

    }

    if(flags & PXG_TEX_EDGECLAMP_X)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    }

    if(flags & PXG_TEX_EDGECLAMP_Y)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    }

    if(flags & PXG_TEX_REPEAT_X)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);

    }

    if(flags & PXG_TEX_REPEAT_Y)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);

    }

    if(flags & PXG_TEX_BORDERCLAMP_X)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

    }

    if(flags & PXG_TEX_BORDERCLAMP_Y)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    }

    if(flags & PXG_FILTER_NEAREST)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    }

    if(flags & PXG_FILTER_LINEAR)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }

    if(flags & PXG_FILTER_MIPMAP_NEAREST || flags & PXG_FILTER_MIPMAP_LINEAR)
    {
        PXG::glGenerateMipmap(GL_TEXTURE_2D);
        if(flags & PXG_FILTER_MIPMAP_NEAREST)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        }

        if(flags & PXG_FILTER_MIPMAP_LINEAR)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }
    }
    glBindTexture(GL_TEXTURE_2D,NULL);
    PXG::Log("pxgTexture::LoadRawHeightmap(): successfully loaded raw heightmap with dimensions: "+std::to_string(width)+"x"+std::to_string(height));
    return true;
}

bool pxgTexture::CreateEmptyTexture(int width, int height, PXG_TEXTURE_FORMAT format)
{
    type = PXG_TEXTURE_2D;
    glGenTextures(1,&name);

    if(name==0)
    {
        PXG::Log("pxgTexture::CreateEmptyTexture(): error generating texture name", ERR);
        return false;
    }

    glBindTexture(GL_TEXTURE_2D,name);

    GLint f;
    GLint i;
    GLint sz;

    switch(format)
    {
        case PXG_TEXTURE_RGB_UB:
            i = GL_RGB;
            f = GL_RGB;
            sz = GL_UNSIGNED_BYTE;
            break;
        case PXG_TEXTURE_RGB_US:
            i = GL_RGB16;
            f = GL_RGB;
            sz = GL_UNSIGNED_SHORT;
            break;
        case PXG_TEXTURE_RGBA_UB:
            i = GL_RGBA;
            f = GL_RGBA;
            sz = GL_UNSIGNED_BYTE;
            break;
        case PXG_TEXTURE_RGBA_US:
            i = GL_RGBA16;
            f = GL_RGBA;
            sz = GL_UNSIGNED_SHORT;
            break;
        case PXG_TEXTURE_R_UB:
            i = GL_R;
            f = GL_R;
            sz = GL_UNSIGNED_BYTE;
            break;
        case PXG_TEXTURE_R_US:
            i = GL_R16;
            f = GL_RED;
            sz = GL_UNSIGNED_SHORT;
            break;
        default:
        PXG::Log("pxgTexture::CreateEmptyTexture() error: unknown format", ERR);
        return false;
    }

    glGetError();
    glTexImage2D(GL_TEXTURE_2D,0,i,width,height,0,f,sz,NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D,0);
    PXG::Log("pxgTexture::CreateEmptyTexture(): created empty texture "+std::to_string(width)+"x"+std::to_string(height)+", texture name: "+std::to_string(name));
    return true;
}

GLuint pxgTexture::GetName()
 {
    return name;
 }

bool pxgTexture::Use(GLuint texStage)
{
    if(name == 0)
       return false;
    glActiveTexture(GL_TEXTURE0+texStage);
    switch(type)
    {
        case PXG_TEXTURE_2D:
            {glBindTexture(GL_TEXTURE_2D,name); break;}
        case PXG_TEXTURE_CUBEMAP:
            {glBindTexture(GL_TEXTURE_CUBE_MAP,name); break;}
        case PXG_TEXTURE_ARRAY:
            {glBindTexture(GL_TEXTURE_2D_ARRAY,name); break;}
    }
    texUnit = texStage;
    return true;
}

void pxgTexture::Unuse()
{
    glActiveTexture(GL_TEXTURE0+texUnit);
        switch(type)
    {
        case PXG_TEXTURE_2D:
            {glBindTexture(GL_TEXTURE_2D,0); break;}
        case PXG_TEXTURE_CUBEMAP:
            {glBindTexture(GL_TEXTURE_CUBE_MAP,0); break;}
        case PXG_TEXTURE_ARRAY:
            {glBindTexture(GL_TEXTURE_2D_ARRAY,0); break;}
    }
    return;
}


void pxgTexture::Delete()
{
    glDeleteTextures(1,&name);
}
