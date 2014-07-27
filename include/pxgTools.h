/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXGTOOLS_H
#define PXGTOOLS_H

#include <string>
#include <glm/fwd.hpp>
#include <GL/gl.h>
#include <GL/glext.h>


bool FileExists(std::string fname);
std::string GetExtension(std::string fname);
std::string GetPath(std::string fname);
std::string GetFileName(std::string fname);
bool  IsRelative(std::string fname);
void pxgInitExtensions();
unsigned long FileSize(std::string fname);
void Print(glm::vec2);
void Print(glm::vec3);
void Print(glm::vec4);

enum PXGLOG_LEVEL
{
    INFO,
    WARN,
    ERR,
    FATAL_ERR,
    OBLIGATORY
};

class PXG
{
public:


    static void InitLog();
    static std::string GetNewMSG() { return m_newLogMSG; }
    static std::string GetLastMSG() { return m_lastLogMSG; }
    static void Log(std::string, PXGLOG_LEVEL type = INFO);




    static PFNGLGENBUFFERSPROC glGenBuffers;
    static PFNGLBINDBUFFERPROC glBindBuffer;
    static PFNGLDELETEBUFFERSPROC glDeleteBuffers;
    static PFNGLBUFFERDATAPROC glBufferData;
    static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
    static PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
    static PFNGLSHADERSOURCEPROC glShaderSource;
    static PFNGLCOMPILESHADERPROC glCompileShader;
    static PFNGLCREATESHADERPROC glCreateShader;
    static PFNGLUSEPROGRAMPROC glUseProgram;
    static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
    static PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;
    static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
    static PFNGLGETSHADERIVPROC glGetShaderiv;
    static PFNGLCREATEPROGRAMPROC glCreateProgram;
    static PFNGLATTACHSHADERPROC glAttachShader;
    static PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation;
    static PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
    static PFNGLLINKPROGRAMPROC glLinkProgram;
    static PFNGLDELETEPROGRAMPROC glDeleteProgram;
    static PFNGLDELETESHADERPROC glDeleteShader;
    static PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
    static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
    static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
    static PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
    static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
    static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
    static PFNGLUNIFORM1FVPROC glUniform1fv;
    static PFNGLUNIFORM1FVPROC glUniform2fv;
    static PFNGLUNIFORM1FVPROC glUniform3fv;
    static PFNGLUNIFORM1FVPROC glUniform4fv;
    static PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
    static PFNGLUNIFORM1IVPROC glUniform1iv;
    static PFNGLUNIFORM1FPROC glUniform1f;
    static PFNGLGETPROGRAMIVPROC glGetProgramiv;
    static PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
    static PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstatnced;
    static PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;
    static PFNGLTEXSTORAGE3DPROC glTexStorage3D;
    static PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
    static PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
    static PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
    static PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
    static PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
    static PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
    static PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
    static PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
    static PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
    static PFNGLDRAWBUFFERSPROC glDrawBuffers;
    static PFNGLTEXSTORAGE2DPROC glTexStorage2D;
    static PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv;
    static PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData;
    static PFNGLGENTRANSFORMFEEDBACKSPROC glGenTransformFeedbacks;
    static PFNGLBINDTRANSFORMFEEDBACKPROC glBindTransfromFeedbacks;
    static PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransfromFeedback;
    static PFNGLENDTRANSFORMFEEDBACKPROC glEndTransformFeedback;
    static PFNGLDRAWTRANSFORMFEEDBACKPROC glDrawTransformFeedback;
    static PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings;

    static PXGLOG_LEVEL log_level;
    static std::ofstream log;

private:
    static std::string m_newLogMSG;
    static std::string m_lastLogMSG;

};

#endif // PXGTOOLS_H
