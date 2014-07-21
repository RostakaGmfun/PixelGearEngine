/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXGSHADER_LIB_H
#define PXGSHADER_LIB_H


#include <vector>
#include <string>

class pxgShader;

#define SHADER(s) #s

enum SL_INIT_FLAGS
{
    SHADER_2D_PASSBY = 1<<1,
};

class pxgShaderLib
{
public:
    pxgShader* InitTextureShader(std::vector<std::string> textures);
    pxgShader* InitTextureShaderAmbient(std::vector<std::string> textures);
    pxgShader* InitLightShader(int numLights);
    pxgShader* InitTexLightShader(std::vector<std::string> textures, int NumLights);
    pxgShader* InitSkyboxShader();
    pxgShader* InitCloudsShader();
    pxgShader* InitTerrainShader();
private:
    std::string TextureMix(std::vector<std::string> textures,std::string outName);
    std::string TexUniforms(std::vector<std::string> textures);
};

#endif //PXGSHADER_LIB_H
