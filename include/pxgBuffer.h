/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXG_BUFFER_H
#define PXG_BUFFER_H

typedef unsigned int GLuint;

enum BUFFER_TYPE
{
    VERTEX = 1,
    ELEMENT
};

class pxgBuffer
{
public:
    ~pxgBuffer();
	bool Init();
	GLuint GetName();
	void SetData(BUFFER_TYPE t,void* data, int sz, int count);
    void GetData(void*& out);
    int GetDataSize();
    bool Use();
   GLuint GetSize() { return size; }
private:
	GLuint name;
	GLuint size;
	int type;
};

#endif //PXG_BUFFER_H
