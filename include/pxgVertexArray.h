#ifndef PXD_VERTEXARRAY
#define PXD_VERTEXARRAY

#include "PXD.h"

class pxdVertexArray
{
public:

    bool Init();
    void Use();
    void Unuse();

private:
    GLuint vao;
};

#endif //PXD_VERTEXARRAY
