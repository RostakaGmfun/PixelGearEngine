/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgBuffer.h>
#include <pxgTools.h>

#include <string>

pxgBuffer::~pxgBuffer()
{
    if(name!=0)
        PXG::glDeleteBuffers(1,&name);
}

bool pxgBuffer::Init()
{
    size = 0;
    PXG::glGenBuffers(1,&name);
	if(name==0)
	{
        PXG::Log("pxgBuffer::Init() error: glGenBuffers() returned zero name for buffer");
        return false;
	}
    PXG::Log("pxgBuffer::Init(): successfully created buffer with name: "+std::to_string(name));
	return true;
}

GLuint pxgBuffer::GetName()
{
	return name;
}

void pxgBuffer::SetData(BUFFER_TYPE t,void* data, int sz, int count)
{
    type = t;
    size = count;
    switch(t)
    {
        case VERTEX:
        {
            PXG::glBindBuffer(GL_ARRAY_BUFFER,name);
            PXG::glBufferData(GL_ARRAY_BUFFER,sz,data,GL_STATIC_DRAW);
            break;
        }
        case ELEMENT:
        {
            PXG::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,name);
            PXG::glBufferData(GL_ELEMENT_ARRAY_BUFFER,sz,data,GL_STATIC_DRAW);
            break;
        }
    }
    return;

}

bool pxgBuffer::Use()
{
    if(name==0)
        return false;
    switch(type)
    {
        case VERTEX:
        {
            PXG::glBindBuffer(GL_ARRAY_BUFFER, name);
            return true;
        }
        case ELEMENT:
        {
            PXG::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,name);
            return true;
        }
        default:
        {
            return false;
        }
    }
    return true;
}

int pxgBuffer::GetDataSize()
{
    int sz = 0;
    if(type==VERTEX)
    {
        PXG::glGetBufferParameteriv(GL_ARRAY_BUFFER,GL_BUFFER_SIZE,&sz);
    }
    if(type==ELEMENT)
    {
        PXG::glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER,GL_BUFFER_SIZE,&sz);
    }
    return sz;
}

void pxgBuffer::GetData(void *&out)
{
    if(type==VERTEX)
        PXG::glGetBufferSubData(GL_ARRAY_BUFFER, 0, GetDataSize(), out);
    if(type==ELEMENT)
        PXG::glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, GetDataSize(), out);
}
