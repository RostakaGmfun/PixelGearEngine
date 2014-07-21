/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgTools.h>
#include <glm/glm.hpp>
#include <fstream>
#include <GL/glx.h>
#include <iostream>

#define GETP(proc,name) PXG::name = (proc)glXGetProcAddress( (GLubyte*)#name ); PXG::Log("\t"#name,INFO);
#define DECLP(proc, name) proc PXG::name = NULL;

PXGLOG_LEVEL PXG::log_level = INFO;

DECLP(PFNGLBINDBUFFERPROC,glBindBuffer)
DECLP(PFNGLGENBUFFERSPROC,glGenBuffers)
DECLP(PFNGLDELETEBUFFERSPROC,glDeleteBuffers)
DECLP(PFNGLBUFFERDATAPROC ,glBufferData)
DECLP(PFNGLGENVERTEXARRAYSPROC,glGenVertexArrays)
DECLP(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray)
DECLP(PFNGLSHADERSOURCEPROC, glShaderSource)
DECLP(PFNGLCOMPILESHADERPROC, glCompileShader)
DECLP(PFNGLCREATESHADERPROC, glCreateShader)
DECLP(PFNGLUSEPROGRAMPROC, glUseProgram)
DECLP(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer)
DECLP(PFNGLVERTEXATTRIBIPOINTERPROC, glVertexAttribIPointer)
DECLP(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray)
DECLP(PFNGLGETSHADERIVPROC, glGetShaderiv)
DECLP(PFNGLCREATEPROGRAMPROC, glCreateProgram)
DECLP(PFNGLATTACHSHADERPROC, glAttachShader)
DECLP(PFNGLBINDFRAGDATALOCATIONPROC, glBindFragDataLocation)
DECLP(PFNGLBINDATTRIBLOCATIONPROC, glBindAttribLocation)
DECLP(PFNGLLINKPROGRAMPROC, glLinkProgram)
DECLP(PFNGLDELETEPROGRAMPROC, glDeleteProgram)
DECLP(PFNGLDELETESHADERPROC, glDeleteShader)
DECLP(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation)
DECLP(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog)
DECLP(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog)
DECLP(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray)
DECLP(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays)
DECLP(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation)
DECLP(PFNGLUNIFORM1FVPROC, glUniform1fv)
DECLP(PFNGLUNIFORM1FVPROC, glUniform2fv)
DECLP(PFNGLUNIFORM1FVPROC, glUniform3fv)
DECLP(PFNGLUNIFORM1FVPROC, glUniform4fv)
DECLP(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv)
DECLP(PFNGLUNIFORM1IVPROC, glUniform1iv)
DECLP(PFNGLUNIFORM1FPROC, glUniform1f)
DECLP(PFNGLGETPROGRAMIVPROC, glGetProgramiv)
DECLP(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap)
DECLP(PFNGLDRAWELEMENTSINSTANCEDPROC, glDrawElementsInstatnced)
DECLP(PFNGLDRAWARRAYSINSTANCEDPROC, glDrawArraysInstanced)
DECLP(PFNGLTEXSTORAGE3DPROC, glTexStorage3D)
DECLP(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers)
DECLP(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer)
DECLP(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers)
DECLP(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D)
DECLP(PFNGLGENRENDERBUFFERSPROC, glGenRenderbuffers)
DECLP(PFNGLBINDRENDERBUFFERPROC, glBindRenderbuffer)
DECLP(PFNGLFRAMEBUFFERRENDERBUFFERPROC, glFramebufferRenderbuffer)
DECLP(PFNGLRENDERBUFFERSTORAGEPROC, glRenderbufferStorage)
DECLP(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus)
DECLP(PFNGLDRAWBUFFERSPROC, glDrawBuffers)
DECLP(PFNGLTEXSTORAGE2DPROC, glTexStorage2D)
DECLP(PFNGLGETBUFFERPARAMETERIVPROC, glGetBufferParameteriv)
DECLP(PFNGLGETBUFFERSUBDATAPROC, glGetBufferSubData)

bool FileExists(std::string fname)
{
    std::ifstream f(fname);
    if(!f.is_open())
        return false;
    else
        return true;
}

std::string GetExtension(std::string fname)
{
    size_t dot = fname.rfind(".");
    if(dot!=std::string::npos)
    {
        int sz = fname.length()-dot;
        return fname.substr(dot+1,sz);
    }
    return "";
}

std::string GetPath(std::string fname)
{
    size_t slash = fname.rfind("/");
    if(slash!=std::string::npos)
    {
        return fname.substr(0,slash+1);
    }
    return "";
}

std::string GetFileName(std::string fname)
{
    size_t dot = fname.rfind(".");
    //size_t slash = fname.rfind("/");
    std::string fnpath =  fname.substr(0,dot);
    size_t slash = fnpath.rfind("/");
    return fnpath.substr(slash+1,fnpath.length());
}

bool IsRelative(std::string fname)
{
    return !fname[0]=='/';
}

unsigned long FileSize(std::string fname)
{
    std::ifstream f(fname);
    if(!f.is_open())
        return 0;
    f.seekg(0,std::ios::end);
    unsigned long sz = f.tellg();
    f.close();
    return sz;
}

void Print(glm::vec2 v)
{
    PXG::Log(std::to_string(v.x)+" | "+std::to_string(v.y));
}

void Print(glm::vec3 v)
{
    PXG::Log(std::to_string(v.x)+" | "+std::to_string(v.y)+" | "+std::to_string(v.z));
}

void Print(glm::vec4 v)
{
    PXG::Log(std::to_string(v.x)+" | "+std::to_string(v.y)+" | "+std::to_string(v.z)+" | "+std::to_string(v.w));
}

void pxgInitExtensions()
{
    PXG::Log("loading extensions...",INFO);
    GETP(PFNGLBINDBUFFERPROC,glBindBuffer);
    GETP(PFNGLGENBUFFERSPROC,glGenBuffers);
    GETP(PFNGLDELETEBUFFERSPROC,glDeleteBuffers);
    GETP(PFNGLBUFFERDATAPROC ,glBufferData);
    GETP(PFNGLGENVERTEXARRAYSPROC,glGenVertexArrays);
    GETP(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
    GETP(PFNGLSHADERSOURCEPROC, glShaderSource);
    GETP(PFNGLCOMPILESHADERPROC, glCompileShader);
    GETP(PFNGLCREATESHADERPROC, glCreateShader);
    GETP(PFNGLUSEPROGRAMPROC, glUseProgram);
    GETP(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
    GETP(PFNGLVERTEXATTRIBIPOINTERPROC, glVertexAttribIPointer);
    GETP(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
    GETP(PFNGLGETSHADERIVPROC, glGetShaderiv);
    GETP(PFNGLCREATEPROGRAMPROC, glCreateProgram);
    GETP(PFNGLATTACHSHADERPROC, glAttachShader);
    GETP(PFNGLBINDFRAGDATALOCATIONPROC, glBindFragDataLocation);
    GETP(PFNGLBINDATTRIBLOCATIONPROC, glBindAttribLocation);
    GETP(PFNGLLINKPROGRAMPROC, glLinkProgram);
    GETP(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
    GETP(PFNGLDELETESHADERPROC, glDeleteShader);
    GETP(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation);
    GETP(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
    GETP(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
    GETP(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
    GETP(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
    GETP(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
    GETP(PFNGLUNIFORM1FVPROC, glUniform1fv);
    GETP(PFNGLUNIFORM1FVPROC, glUniform2fv);
    GETP(PFNGLUNIFORM1FVPROC, glUniform3fv);
    GETP(PFNGLUNIFORM1FVPROC, glUniform4fv);
    GETP(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
    GETP(PFNGLUNIFORM1IVPROC, glUniform1iv);
    GETP(PFNGLUNIFORM1FPROC, glUniform1f);
    GETP(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
    GETP(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap);
    GETP(PFNGLDRAWELEMENTSINSTANCEDPROC, glDrawElementsInstatnced);
    GETP(PFNGLDRAWARRAYSINSTANCEDPROC, glDrawArraysInstanced);
    GETP(PFNGLTEXSTORAGE3DPROC, glTexStorage3D);
    GETP(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers)
    GETP(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer)
    GETP(PFNGLDELETEFRAMEBUFFERSPROC, glDeleteFramebuffers)
    GETP(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D)
    GETP(PFNGLGENRENDERBUFFERSPROC, glGenRenderbuffers)
    GETP(PFNGLBINDRENDERBUFFERPROC, glBindRenderbuffer)
    GETP(PFNGLFRAMEBUFFERRENDERBUFFERPROC, glFramebufferRenderbuffer)
    GETP(PFNGLRENDERBUFFERSTORAGEPROC, glRenderbufferStorage)
    GETP(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus)
    GETP(PFNGLDRAWBUFFERSPROC, glDrawBuffers)
    GETP(PFNGLTEXSTORAGE2DPROC, glTexStorage2D)
    GETP(PFNGLGETBUFFERPARAMETERIVPROC, glGetBufferParameteriv)
    GETP(PFNGLGETBUFFERSUBDATAPROC, glGetBufferSubData)
    PXG::Log("Initialised extensions", INFO);
}

std::ofstream PXG::log;
std::string PXG::m_newLogMSG;
std::string PXG::m_lastLogMSG;

void PXG::Log(std::string text, PXGLOG_LEVEL lvl)
{
    if(lvl<log_level)
        return;
    std::cout << text << std::endl;
    log << text << std::endl;
    m_lastLogMSG = m_newLogMSG;
    m_newLogMSG = text;
}

void PXG::InitLog()
{

    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);

    int day = aTime->tm_mday;
    int month = aTime->tm_mon + 1;
    int year = aTime->tm_year + 1900;
    using namespace std;
    string lname = string(to_string(day))+"-"+std::string(to_string(month))+"-"+std::string(to_string(year))+"-pxg.log";
    log.open(lname);
}
