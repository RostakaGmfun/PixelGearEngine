/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXG_SHADER
#define PXG_SHADER

#include <pxgTexture.h>
#include <pxgVertexLayouts.h>
#include <pxgUniforms.h>

#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <fstream>
#include <pxgVertexLayouts.h>

class pxgLight;

enum PXG_VERTEX_ELEMENT
{
    PXG_POSITION,
    PXG_TEXCOORD,
    PXG_NORMAL,
    PXG_BONE_INDEX,
    PXG_BONE_WEIGHT
};

enum PXG_FORMAT
{
    PXG_1F,
    PXG_2F,
    PXG_3F,
    PXG_4F,
    PXG_4FM,
    PXG_1I
};

struct __Uniform
{
    GLint id;
    PXG_FORMAT format;
};

struct __Attribute
{
    std::string name;
    GLuint loc;
};

struct __lightUniform
{
    __lightUniform(GLint loc, int i): location(loc), index(i) {}
    GLint location;
    int index;
};

class pxgShader
{
public:

	~pxgShader();
	pxgShader();
	bool LoadFromFile(const char fileName);
	bool VS(const char **source);
	void SetVertexLayout(PXG_VERTEX_LAYOUT layout, std::vector<std::string> attributes);
	bool FS(const char **source);
	bool Link(PXG_VERTEX_LAYOUT layout, std::vector<std::string> attributeNames);
	GLuint GetProgram() const { return program; }
	GLuint GetVS() const { return vs; }
	GLuint GetFS() const { return fs; }
	bool Use();
	void UseTexture(GLint stage, std::string name);
	bool AddTexture(PXG_TEXTURE_STAGE stage, std::string textureName);
	void SetTexture(PXG_TEXTURE_STAGE stage, pxgTexture* tex);
	void Unuse();

	void AddUniform(std::string name, PXG_FORMAT format);
	void SetUniformTarget(std::string name, PXG_UNIFORM type);
	void SetUniform(std::string name, const void* data);
	void SetUniform(PXG_UNIFORM type, const void* data);

	void AddLightUniform(int numLights, std::string name);
	void SetLight(int lightIndex, pxgLight* light);
	void SetLightCount(int numLights);

	std::string GetVSLog();
	std::string GetFSLog();
	std::string GetProgramLog();
	void Destroy();
private:
	const char *vsSource,*fsSource;
	GLuint vs, fs, program;
	std::map<PXG_UNIFORM, std::string> uniformTargets;
    std::map<std::string, __Uniform*> uniforms;

    std::vector<__Attribute*> attributes;
    PXG_VERTEX_LAYOUT vertexLayout;

    std::map<PXG_TEXTURE_STAGE,std::string> textures;

    std::string lightsName;
    int lightCount;
    std::vector<GLint> positions;
    std::vector<GLint> directions;
    std::vector<GLint> types;
    std::vector<GLint> diffuses;
    std::vector<GLint> attenuations;
    GLint u_numLights;
};

#endif //PXG_SHADER
