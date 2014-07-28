/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgShader.h>
#include <pxgLight.h>
#include <pxgTools.h>

#include <glm/gtc/type_ptr.hpp>

using namespace std;

pxgShader::pxgShader():lightCount(0), vs(0), gs(0), fs(0), program(0), u_numLights(0)
{
}
pxgShader::~pxgShader()
{
	Destroy();
}

bool pxgShader::VS(const char **src)
{
	vs = PXG::glCreateShader(GL_VERTEX_SHADER);

	if(vs==0)
	{
	    PXG::Log("pxgShader::VS(): failed to create shader object",ERR);
		return false;
	}
    PXG::Log("pxgShader::VS(): created shader object with name: "+std::to_string(vs));
	PXG::glShaderSource(vs,1,src,NULL);
	PXG::glCompileShader(vs);

	GLint status;
	PXG::glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE)
	{
	    PXG::Log("pxgShader::VS(): error compiling shader",ERR);
	    PXG::Log(GetVSLog(),ERR);
		return false;
	}
	PXG::Log("pxgShader::VS(): successfully compiled shader");
	return true;
}

void pxgShader::SetVertexLayout(PXG_VERTEX_LAYOUT layout, std::vector<std::string> attributeNames)
{
    if(attributeNames.size()<=0)
        return;
    //vertexLayout = layout;
    if(layout==PXG_VERTEX3DTANIM)
    {
        PXG::glBindAttribLocation(program,PXG_POSITION,attributeNames[0].c_str());
        PXG::glBindAttribLocation(program,PXG_TEXCOORD,attributeNames[1].c_str());
        PXG::glBindAttribLocation(program,PXG_BONE_INDEX,attributeNames[2].c_str());
        PXG::glBindAttribLocation(program,PXG_BONE_WEIGHT,attributeNames[3].c_str());
        return;
    }
    for(int i = 0;i<attributeNames.size();i++)
    {
        //if(PXG_POSITIO)
        PXG::glBindAttribLocation(program,PXG_POSITION+i,attributeNames[PXG_POSITION+i].c_str());
    }
}

bool pxgShader::GS(const char **source)
{
    gs = PXG::glCreateShader(GL_GEOMETRY_SHADER);

    if(gs==0)
    {
        PXG::Log("pxgShader::GS(): failed to create shader object",ERR);
        return false;
    }
    PXG::Log("pxgShader::GS(): created shader object with name: "+std::to_string(gs));
    PXG::glShaderSource(gs,1,source,NULL);
    PXG::glCompileShader(gs);

    GLint status;
    PXG::glGetShaderiv(gs, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE)
    {
        PXG::Log("pxgShader::GS(): error compiling shader",ERR);
        PXG::Log(GetGSLog(),ERR);
        return false;
    }
    PXG::Log("pxgShader::GS(): successfully compiled shader");
    return true;
}

bool pxgShader::FS(const char **src)
{
	fs = PXG::glCreateShader(GL_FRAGMENT_SHADER);
	if(fs==0)
	{
	    PXG::Log("pxgShader::FS(): failed to create shader object",ERR);
		return false;
	}
	PXG::Log("pxgShader::FS(): created shader object with name: "+std::to_string(fs));
	PXG::glShaderSource(fs,1,src,NULL);
	PXG::glCompileShader(fs);

    GLint status;
	PXG::glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE)
	{
	    PXG::Log("pxgShader::FS(): error compiling shader",ERR);
	    PXG::Log(GetFSLog(),ERR);
		return false;
	}
	PXG::Log("pxgShader::FS(): successfully compiled shader");
	return true;
}

bool pxgShader::Link(PXG_VERTEX_LAYOUT layout, std::vector<std::string> attributeNames)
{
	vertexLayout = layout;
	program = PXG::glCreateProgram();
	if(program==0)
    {
        PXG::Log("pxgShader::Link(): error creating programm object", ERR);
        return false;
    }
    if(vs)
        PXG::glAttachShader(program,vs);
    if(gs)
        PXG::glAttachShader(program,gs);
    if(fs)
        PXG::glAttachShader(program,fs);
	SetVertexLayout(vertexLayout,attributeNames);
	PXG::glLinkProgram(program);
	GLint status;
	PXG::glGetProgramiv(program, GL_LINK_STATUS, &status);
	if(status != GL_TRUE)
	{
        PXG::Log("pxgShader::Link(): error linking shader",ERR);
        PXG::Log(GetProgramLog(),ERR);
		return false;
	}
	PXG::Log("pxgShader::Link(): successfully linked program with name: "+ std::to_string(program));
	return true;
}

bool pxgShader::LinkTransformFeedback(PXG_VERTEX_LAYOUT layout, std::vector<std::string> feedbackVaryings, std::vector<std::string> attributeNames)
{
    if(vs==0)
    {
        PXG::Log("pxgShader::LinkTransformFeedback() error: vertex shader not compiled", ERR);
        return false;
    }

    vertexLayout = layout;

    program = PXG::glCreateProgram();
    if(!program)
    {
        PXG::Log("pxgShader::LinkTransformFeedback(): error creating shader program object", ERR);
        return false;
    }

    for(int i = 0; i<feedbackVaryings.size();i++)
    {
        PXG::glBindAttribLocation(program,PXG_POSITION+i,attributeNames[PXG_POSITION+i].c_str());
    }

    PXG::glTransformFeedbackVaryings(program,feedbackVaryings.size(),(const GLchar* const *)&feedbackVaryings[0], GL_INTERLEAVED_ATTRIBS);

    if(vs)
        PXG::glAttachShader(program, vs);
    if(gs)
        PXG::glAttachShader(program,gs);
    PXG::glLinkProgram(program);
    GLint status;
    PXG::glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status != GL_TRUE)
    {
        PXG::Log("pxgShader::LinkTransfromFeedback(): error linking shader",ERR);
        PXG::Log(GetProgramLog(),ERR);
        return false;
    }
    PXG::Log("pxgShader::LinkTransformFeedback(): successfully linked program with name: "+ std::to_string(program));
    return true;
}

void pxgShader::Destroy()
{
	PXG::glDeleteProgram(program);
	PXG::glDeleteShader(vs);
	PXG::glDeleteShader(fs);
}

std::string pxgShader::GetVSLog()
{
    char *log;
    GLint length, result;
    PXG::glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &length);
    if(length<=0)
        return "";
    log = new char[length];
    PXG::glGetShaderInfoLog(vs, length, &result, log);
    return log;
}

std::string pxgShader::GetGSLog()
{
    char *log;
    GLint length, result;
    PXG::glGetShaderiv(gs, GL_INFO_LOG_LENGTH, &length);
    if(length<=0)
        return "";
    log = new char[length];
    PXG::glGetShaderInfoLog(gs, length, &result, log);
    return log;
}

std::string pxgShader::GetFSLog()
{
    char *log;
    GLint length, result;
    PXG::glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &length);
    if(length<=0)
        return "";
    log = new char[length];
    PXG::glGetShaderInfoLog(fs, length, &result, log);
    return log;
}

std::string pxgShader::GetProgramLog()
{
    char *log;
    GLint length, result;
    PXG::glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    if(length<=0)
        return "";
    log = new char[length];
    PXG::glGetProgramInfoLog(program, length, &result, log);
    return log;
}

bool pxgShader::Use()
{
    if(program==0)
        return false;
    GLint id;
    glGetIntegerv(GL_CURRENT_PROGRAM,&id);
    if(id!=program)
        PXG::glUseProgram(program);
    return true;
}

void pxgShader::Unuse()
{
    GLint id;
    glGetIntegerv(GL_CURRENT_PROGRAM,&id);
    if(program==id)
        PXG::glUseProgram(NULL);
}

void pxgShader::AddUniform(std::string name, PXG_FORMAT format)
{
    if(program == NULL)
    {
        PXG::Log("pxgShader::AddUniform(): shader program is not linked",ERR);
        return;
    }

    PXG::Log("pxgShader::AddUniform: getting uniform location: " + name);
    GLint uniform = PXG::glGetUniformLocation(program, name.c_str());
    if(uniform<0)
    {
        PXG::Log("pxgShader::AddUniform(): error getting uniform location for: " + name,ERR);
        return;
    }
    __Uniform *u = new __Uniform;
    u->format = format;
    u->id = uniform;
    uniforms[name] = u;
        return;
}

void pxgShader::SetUniformTarget(std::string name, PXG_UNIFORM type)
{
    uniformTargets[type] = name;
}

void pxgShader::SetUniform(std::string name, const void* data)
{
    if(uniforms.find(name)==uniforms.end())
    {
        //PXG::Log("pxgShader::SetUniform(): uniform does not exist: "+name,WARN);
        return;
    }
    Use();
    switch(uniforms[name]->format)
    {
        case PXG_1F:
        {
            PXG::glUniform1fv(uniforms[name]->id,1,(GLfloat*)data);
            break;
        }
        case PXG_2F:
        {
            PXG::glUniform2fv(uniforms[name]->id,1,(GLfloat*)data);
            break;
        }
        case PXG_3F:
        {
            PXG::glUniform3fv(uniforms[name]->id,1,(GLfloat*)data);
            break;
        }
        case PXG_4F:
        {
            PXG::glUniform4fv(uniforms[name]->id,1,(GLfloat*)data);
            break;
        }
        case PXG_4FM:
        {
            PXG::glUniformMatrix4fv(uniforms[name]->id,1,GL_FALSE,(GLfloat*)data);
            break;
        }
        case PXG_1I:
        {
            PXG::glUniform1iv(uniforms[name]->id,1,(GLint*)data);
        }
        default:
        {
            break;
        }
    }
    //Unuse();
    return;
}

void pxgShader::SetUniform(PXG_UNIFORM type, const void* data)
{
    if(uniformTargets.find(type)==uniformTargets.end())
    {
        //PXG::Log("pxgShader::SetUniform(): no target found for target number " + std::to_string(type), ERR);
        return;
    }
    SetUniform(uniformTargets[type],data);
    return;
}

bool pxgShader::AddTexture(PXG_TEXTURE_STAGE stage, std::string texName)
{
    textures[stage] = texName;
    AddUniform(texName,PXG_1I);
    return true;
}

void pxgShader::SetTexture(PXG_TEXTURE_STAGE stage, pxgTexture* texture)
{
    if(textures[stage].empty())
        return;
    //PXG::Log("hah");
   /* if(texture==NULL)
        glBindTexture(GL_TEXTURE_2D,0);*/
    else
        texture->Use((int)stage);
    SetUniform(textures[stage],&stage);
    return;

}

void pxgShader::UseTexture(GLint stage, std::string name)
{
    Use();
    GLint  tex = PXG::glGetUniformLocation(program,name.c_str());
    if(tex<0)
    {
        PXG::Log("pxgShader::UseTexture():texture with name "+name+" does not exist!",ERR);
        return;
    }
    PXG::glUniform1iv(tex,1,&stage);
    return;
}

void pxgShader::AddLightUniform(int numLights,std::string name)
{

    GLint u = -1;
    lightCount = numLights;
    lightsName = name;
    for(int i = 0;i<numLights;i++)
    {
        u = PXG::glGetUniformLocation(program, std::string(name+"["+to_string(i)+"].pos").c_str());
        if(u<0)
        {
            PXG::Log("pxgShader::AddLightUniform() error: cannot get uniform location for: "+std::string(name+"["+to_string(i)+"].pos"),ERR);
        }
        positions.push_back(u);

        u = PXG::glGetUniformLocation(program, std::string(name+"["+to_string(i)+"].dir").c_str());
        if(u<0)
        {
            PXG::Log("pxgShader::AddLightUniform() error: cannot get uniform location for: "+std::string(name+"["+to_string(i)+"].dir"),ERR);
        }
        directions.push_back(u);

        u = PXG::glGetUniformLocation(program, std::string(name+"["+to_string(i)+"].type").c_str());
        if(u<0)
        {
            PXG::Log("pxgShader::AddLightUniform() error: cannot get uniform location for: "+std::string(name+"["+to_string(i)+"].type"),ERR);
        }
        types.push_back(u);

        u = PXG::glGetUniformLocation(program, std::string(name+"["+to_string(i)+"].diffuse").c_str());
        if(u<0)
        {
            PXG::Log("pxgShader::AddLightUniform() error: cannot get uniform location for: "+std::string(name+"["+to_string(i)+"].diffuse"),ERR);
        }
        diffuses.push_back(u);

        u = PXG::glGetUniformLocation(program, std::string(name+"["+to_string(i)+"].attenuation").c_str());
        if(u<0)
        {
            PXG::Log("pxgShader::AddLightUniform() error: cannot get uniform location for: "+std::string(name+"["+to_string(i)+"].attenuation"),ERR);
        }
        attenuations.push_back(u);
    }
        u_numLights = PXG::glGetUniformLocation(program, "numLights");
        if(u<0)
        {
            PXG::Log("pxgShader::AddLightUniform() error: cannot get uniform location for: numLigths",ERR);
        }
        Use();
        PXG::glUniform1iv(u_numLights,1,&lightCount);
}

void pxgShader::SetLight(int lightIndex, pxgLight* light)
{
    if(lightIndex<0||light==NULL)
        return;
    if(lightIndex>=lightCount)
    {
        PXG::Log("pxgShader::SetLight() error: to big light index: "+to_string(lightIndex)+" max lights: "+to_string(lightCount),ERR);
        return;
    }
    Use();
    PXG::glUniform1iv(u_numLights,1,&lightCount);
    switch(light->GetType())
    {
        case PXG_LIGHT_DIRECTIONAL:
        {
            int t = 0;
            PXG::glUniform3fv(directions[lightIndex],1,glm::value_ptr(light->GetRotation()));
            PXG::glUniform3fv(diffuses[lightIndex],1,glm::value_ptr(light->GetLightColor()));
            PXG::glUniform1iv(types[lightIndex],1,&t);
            break;
        }
        case PXG_LIGHT_POINT:
        {
            int t = 1;
            PXG::glUniform3fv(positions[lightIndex],1,glm::value_ptr(light->GetTranslation()));
            PXG::glUniform3fv(diffuses[lightIndex],1,glm::value_ptr(light->GetLightColor()));
            PXG::glUniform1iv(types[lightIndex],1,&t);
            GLfloat a = light->GetAttenuation();
            PXG::glUniform1fv(attenuations[lightIndex],1,&a);
            break;
        }
    }
}

void pxgShader::SetLightCount(int numLights)
{
    Use();
    PXG::glUniform1iv(u_numLights,1,&numLights);
}
