#include <pxdVertexArray.h>

bool pxdVertexArray::Init()
{
    if(vao!=0)
        PXD::glDeleteVertexArrays(1,&vao);
    PXD::glGenVertexArrays(1,&vao);
    if(vao==0)
    {
        PXD::Log("pxdVertexArray::Init(): error creating vertex array object");
        return false;
    }
    return true;
}

void pxdVertexArray::Use()
{
    if(vao == 0)
        return;
    PXD::glBindVertexArray(vao);
}

void pxdVertexArray::Unuse()
{
    PXD::glBindVertexArray(0);
}
