/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgShaderLib.h>
#include <pxgShader.h>
#include <pxgTools.h>

#include <sstream>

namespace __shaderlib_internal
{
const char *vertex2dpassby =
    "#version 130 \n"
    "in vec2 position;"
    "void main() {"
    "   gl_Position = vec4(position, 0.0, 1.0);"
    "}";
const char* fragment2dpassby =
    "#version 130 \n"
    "out vec4 outColor;"
    "void main() {"
    "   outColor = vec4(0.7, 1, 0.7, 1.0);"
    "}";

    std::string lightStruct =
    SHADER(
    struct pxgLight
    {
    vec3 pos;
    vec3 dir;
    vec3 diffuse;
    float shinisses;
    float attenuation;
    int type;
    };
    );

    std::string materialStruct =
    SHADER(
    struct pxgMaterial
    {
        vec3 diffuse;
        vec3 ambient;
        vec3 specular;
        float shininess;
    };
    );

    std::string lighting =
    SHADER(
    vec3 pxgLighting(in pxgLight light, in vec3 normal, in vec3 vpos, in vec3 viewPos, in pxgMaterial mat)
    {
        switch(light.type)
        {
            case 0: /* directional light */
            {
                vec3 n = normalize(normal);
                vec3 l = normalize(light.dir);
                vec3 v = normalize(viewPos - vpos);
                vec3 h = normalize(normalize(vpos)+l);
                float NdotL = max(dot(n,l),0);
                if(mat.specular.x+mat.specular.y+mat.specular.z<=0)
                    return (light.diffuse*NdotL)+mat.ambient;
                float specularC = 0.0;
                if(NdotL>0.0)
                {
                    specularC = pow(max(dot(h,n),0.0), mat.shininess);
                    specularC = clamp(specularC,0,1);
                }
                return (light.diffuse*NdotL+specularC*mat.specular)+mat.ambient;
            }
            case 1: /* point light */
            {
                float d = distance(vpos,light.pos);
                vec3 n = normalize(normal);
                vec3 l = normalize(light.pos - vpos);
                vec3 v = normalize(viewPos - vpos);
                vec3 h = normalize(normalize(-vpos)+n);
                float NdotL = max(dot(l,normal),0);
                if(mat.specular.x+mat.specular.y+mat.specular.z<=0)
                    return (light.diffuse*NdotL)/(light.attenuation*d+light.attenuation*d*d)+mat.ambient;
                float specularC = 0.0;
                if(NdotL>0.0)
                {
                    specularC = pow(max(0.0, dot(h, n)), mat.shininess);
                    specularC = clamp(specularC,0,1);
                }
                if(light.attenuation==0) light.attenuation=0.01;
                return (light.diffuse*NdotL+specularC*mat.specular)/(light.attenuation*d+light.attenuation*d*d)+mat.ambient;
            }
        }
    }

    );
} //__shaderlib_internal

pxgShader* pxgShaderLib::InitTextureShader(std::vector<std::string> textures)
{

    if(textures.size()<=0)
    {
        PXG::Log("pxgShaderLib::InitTextureShader() error: no textures passed");
        return NULL;
    }

    const char* vs_source = "#version 130\n"
                 SHADER(
                        in vec3 position;
                        in vec2 texCoord;
                        out vec2 TexCoord;
                        uniform mat4 model;
                        uniform mat4 VP;
                        void main()
                        {
                            gl_Position = VP*model*vec4(position, 1.0);
                            TexCoord = texCoord;
                        }
                    );
    std::string texUniforms;
    for(int i = 0;i<textures.size();i++)
    {
        texUniforms+="uniform sampler2D " + textures[i] + ";\n";
    }
    std::string texMix;
    if(textures.size()==1)
    {
        texMix = "outColor = texture("+textures[0]+",TexCoord);\n";
    }
    else
    if(textures.size()==2)
    {
        texMix = "outColor = mix(texture("+textures[0]+", TexCoord),texture("+textures[1]+",TexCoord),0.5);\n";
    }
    else
    {
        std::string c = "vec4 temp = mix(texture("+textures[0]+",TexCoord),texture("+textures[1]+",TexCoord),0.5);\n";
        for(int i = 2;i<textures.size();i++)
        {
            c+= "temp = mix(temp,texture("+textures[i]+",TexCoord),0.5);\n";
        }
        texMix = c + "outColor = temp;";
    }

    std::string fs_source = std::string("#version 130\n")+
                 std::string(SHADER(in vec2 TexCoord;
                        out vec4 outColor;))+texUniforms+
                std::string(SHADER(
                        void main()
                        {
                            ))+texMix+SHADER(
                            if(outColor.a<=0.005) discard;
                        }
                        );
    //PXG::Log(fs_source);

    pxgShader* shader = new pxgShader;
    shader->VS(&vs_source);
    const char* fs = fs_source.c_str();
    shader->FS(&fs);
    std::vector<std::string> attribs;
    attribs.push_back("position");
    attribs.push_back("texCoord");
    shader->Link(PXG_VERTEX3DT,attribs);
    shader->AddUniform("model", PXG_4FM);
    shader->SetUniformTarget("model",PXG_MODEL);
    shader->AddUniform("VP", PXG_4FM);
    shader->SetUniformTarget("VP",PXG_VIEW_PROJECTION);
    return shader;
}

pxgShader* pxgShaderLib::InitTextureShaderAmbient(std::vector<std::string> textures)
{

    if(textures.size()<=0)
    {
        PXG::Log("pxgShaderLib::InitTextureShaderAmbient() error: no textures passed");
        return NULL;
    }

    const char* vs_source = "#version 130\n"
                 SHADER(in vec3 position;
                        in vec2 texCoord;
                        out vec2 TexCoord;
                        uniform mat4 model;
                        uniform mat4 VP;
                        void main()
                        {
                            gl_Position = VP*model*vec4(position, 1.0);
                            TexCoord = texCoord;
                        }
                    );
    std::string texUniforms;
    for(int i = 0;i<textures.size();i++)
    {
        texUniforms+="uniform sampler2D " + textures[i] + ";\n";
    }
    std::string texMix;
    if(textures.size()==1)
    {
        texMix = "outColor = texture("+textures[0]+",TexCoord);\n";
    }
    else
    if(textures.size()==2)
    {
        texMix = "outColor = mix(texture("+textures[0]+", TexCoord),texture("+textures[1]+",TexCoord),0.5);\n";
    }
    else
    {
        std::string c = "vec4 temp = mix(texture("+textures[0]+",TexCoord),texture("+textures[1]+",TexCoord),0.5);\n";
        for(int i = 2;i<textures.size();i++)
        {
            c+= "temp = mix(temp,texture("+textures[i]+",TexCoord),0.5);\n";
        }
        texMix = c + "outColor = temp;";
    }

    std::string fs_source = std::string("#version 130\n")+
                 std::string(SHADER(in vec2 TexCoord;
                        out vec4 outColor;))+texUniforms+"uniform vec4 ambient;"+
                std::string(SHADER(
                        void main()
                        {
                            ))+texMix+SHADER(
                            if(outColor.a<=0.005) discard;
                            outColor+=ambient;
                        }
                        );
   // PXG::Log(fs_source);

    pxgShader* shader = new pxgShader;
    shader->VS(&vs_source);
    const char* fs = fs_source.c_str();
    shader->FS(&fs);
    std::vector<std::string> attribs;
    attribs.push_back("position");
    attribs.push_back("texCoord");
    shader->Link(PXG_VERTEX3DT,attribs);
    shader->AddUniform("model", PXG_4FM);
    shader->SetUniformTarget("model",PXG_MODEL);
    shader->AddUniform("VP", PXG_4FM);
    shader->SetUniformTarget("VP",PXG_VIEW_PROJECTION);
    shader->AddUniform("ambient", PXG_4F);
    shader->SetUniformTarget("ambient",PXG_AMBIENT);

    for(int i = 0;i<textures.size();i++)
    {
        shader->AddTexture((PXG_TEXTURE_STAGE)i,textures[i]);
    }
    return shader;
}

pxgShader* pxgShaderLib::InitTexLightShader(std::vector<std::string> textures, int numLights)
{
    using namespace __shaderlib_internal;

    if(textures.size()<=0)
    {
        PXG::Log("pxgShaderLib::InitTexLightShader() error: no textures passed");
        return NULL;
    }

    if(numLights<=0)
    {
        PXG::Log("pxgShaderLib::InitTexLightShader() error: no lights passed");
        return NULL;
    }

    /*if(numLights>8)
    {
        PXG::Log("pxgShaderLib::InitTexLightShader() error: too much lights. PXG supports only 8 lights so far");
        return NULL;
    }*/


    std::string vs_source =
    "#version 130\n"
    SHADER(
            in vec3 position;
            in vec2 texCoord;
            in vec3 normal;
            out vec2 TexCoord;
            out vec3 N;
            out vec3 vp;
            out vec3 view;
            uniform mat4 model;
            uniform mat4 V;
            uniform mat4 P;
            void main()
            {
                gl_Position = P*V*model*vec4(position, 1.0);
                TexCoord = texCoord;
                N = mat3(model)*normal;
                vp = vec3(model*vec4(position,1));
                view = vec3(V[3]);
            }
    );

    std::string fs_source =
    "#version 130\n"
    +lightStruct+materialStruct
    +lighting+
    SHADER(
    in vec2 TexCoord;
    in vec3 N;
    in vec3 vp;
    in vec3 view;
    uniform int useTexture;
    )+TexUniforms(textures)+SHADER(
    out vec4 outColor;
    uniform int numLights;
    uniform vec3 sceneAmbient;
    uniform pxgMaterial material;
    const int maxLights = )+std::to_string(numLights)+SHADER(;
    uniform pxgLight lights[)+std::to_string(numLights)+SHADER(];
    void main()
    {
        vec4 temp;
        if(useTexture>0)
        {
        )+TextureMix(textures,"temp")+std::string(
       "outColor = temp;"
       "outColor.a = temp.a;"
       "}"
      " else"
      "   outColor = vec4(material.diffuse+material.ambient,1);"
       "if(outColor.a<=0.005) discard;"
       "clamp(numLights,0,maxLights);"
        "for(int i = 0;i<numLights;i++)"
       " { "
       "    outColor+=vec4(pxgLighting(lights[i],N,vp,view,material),0);"
       " } "
       "outColor+=vec4(sceneAmbient,1);"
    "}");

   // PXG::Log(fs_source);

    pxgShader* shader = new pxgShader;
    const char* vs = vs_source.c_str();
    shader->VS(&vs);
    const char* fs = fs_source.c_str();
    shader->FS(&fs);
    std::vector<std::string> attribs;
    attribs.push_back("position");
    attribs.push_back("texCoord");
    attribs.push_back("normal");
    shader->Link(PXG_VERTEX3DTN,attribs);

    shader->AddUniform("model", PXG_4FM);
    shader->SetUniformTarget("model",PXG_MODEL);
    shader->AddUniform("V", PXG_4FM);
    shader->SetUniformTarget("V",PXG_VIEW);
    shader->AddUniform("P", PXG_4FM);
    shader->SetUniformTarget("P",PXG_PROJECTION);

    shader->AddUniform("useTexture",PXG_1I);
    shader->SetUniformTarget("useTexture",PXG_USE_TEXTURE);

    shader->AddUniform("material.ambient",PXG_3F);
    shader->SetUniformTarget("material.ambient",PXG_AMBIENT);

    shader->AddUniform("material.diffuse",PXG_3F);
    shader->SetUniformTarget("material.diffuse",PXG_DIFFUSE);

    shader->AddUniform("material.specular",PXG_3F);
    shader->SetUniformTarget("material.specular",PXG_SPECULAR);

    shader->AddUniform("material.shininess", PXG_1F);
    shader->SetUniformTarget("material.shininess", PXG_SHININESS);

    shader->AddUniform("sceneAmbient",PXG_3F);
    shader->SetUniformTarget("sceneAmbient",PXG_SCENE_AMBIENT);

    shader->AddLightUniform(numLights,"lights");
    for(int i = 0;i<textures.size();i++)
    {
        shader->AddTexture((PXG_TEXTURE_STAGE)i,textures[i]);
    }

    return shader;

}

std::string pxgShaderLib::TextureMix(std::vector<std::string> textures, std::string outName)
{
    if(outName.length()<=0)
    {
        return "";
    }

    std::string out;

    if(textures.size()==1)
    {
        out = "vec4 "+outName + " = texture("+textures[0]+",TexCoord);\n";
    }
    else
    if(textures.size()==2)
    {
        out = "vec4 " + outName + " = mix(texture("+textures[0]+", TexCoord),texture("+textures[1]+",TexCoord),0.5);\n";
    }
    else
    {
        std::string c = "vec4 temp = mix(texture("+textures[0]+",TexCoord),texture("+textures[1]+",TexCoord),0.5);\n";
        for(int i = 2;i<textures.size();i++)
        {
            c+= outName+" = mix(temp,texture("+textures[i]+",TexCoord),0.5);\n";
        }
        out = c;
    }
    return out;

}

std::string pxgShaderLib::TexUniforms(std::vector<std::string> textures)
{
    std::string texUniforms;
    for(int i = 0;i<textures.size();i++)
    {
        texUniforms+="uniform sampler2D " + textures[i] + ";\n";
    }

    return texUniforms;
}

pxgShader* pxgShaderLib::InitSkyboxShader()
{
    std::string vs_source =
    "#version 130\n"
    "in vec3 position;"
    "in vec2 tx;"
    "uniform mat4 M;"
    "uniform mat4 VP;"
    "out vec3 texCoord;"
    "void main()"
    "{"
    "   texCoord = position;"
    "   vec4 transformedPos =  VP*M*vec4(position,1.0);"
    "   gl_Position = transformedPos.xyww;"
    "}";

    std::string fs_source =
    "#version 130\n"
    SHADER(
    in vec3 texCoord;
    out vec4 outColor;
    uniform samplerCube cubemap;
    void main()
    {
        outColor = texture(cubemap,texCoord);
        //outColor = vec4(1);
    }
           );

    pxgShader* shader = new pxgShader;
    const char* vs = vs_source.c_str();
    shader->VS(&vs);
    const char* fs = fs_source.c_str();
    shader->FS(&fs);
    std::vector<std::string> attribs;
    attribs.push_back("position");
    attribs.push_back("tx");
    shader->Link(PXG_VERTEX3DT,attribs);
    shader->AddUniform("M", PXG_4FM);
    shader->SetUniformTarget("M",PXG_MODEL);
    shader->AddUniform("VP", PXG_4FM);
    shader->SetUniformTarget("VP",PXG_VIEW_PROJECTION);
    shader->AddTexture(PXG_TEXTURE0,"cubemap");
    //shader->AddTexture(PXG_TEXTURE1, "clouds");
    return shader;
}


pxgShader* pxgShaderLib::InitCloudsShader()
{
    std::string vs_source =
    "#version 130\n"
    "in vec3 position;"
    "in vec2 tx;"
    "uniform mat4 M;"
    "uniform mat4 VP;"
    "out vec2 texCoord;"
    "void main()"
    "{"
    "   texCoord = tx;"
    "   vec4 transformedPos =  VP*M*vec4(position,1.0);"
    "   gl_Position = transformedPos.xyww;"
    "}";

    std::string fs_source =
    "#version 130\n"
    "in vec2 texCoord;"
    "out vec4 outColor;"
    "uniform sampler2D clouds;"
    "uniform vec2 move;"
    "void main()"
    "{"
    "outColor = vec4(1,1,1,1);"
    "outColor.a = texture(clouds,texCoord*5+move);"
    "outColor.a*=0.5-sqrt(pow(texCoord.x-0.5,2)+pow(texCoord.y-0.5,2));"
    "}";

    pxgShader* shader = new pxgShader;
    const char* vs = vs_source.c_str();
    shader->VS(&vs);
    const char* fs = fs_source.c_str();
    shader->FS(&fs);
    std::vector<std::string> attribs;
    attribs.push_back("position");
    attribs.push_back("tx");
    shader->Link(PXG_VERTEX3DT,attribs);
    shader->AddUniform("M", PXG_4FM);
    shader->SetUniformTarget("M",PXG_MODEL);
    shader->AddUniform("VP", PXG_4FM);
    shader->AddUniform("move",PXG_2F);
    shader->SetUniformTarget("VP",PXG_VIEW_PROJECTION);
    //shader->AddTexture(PXG_TEXTURE0,"cubemap");
    shader->AddTexture(PXG_TEXTURE0, "clouds");
    return shader;
}


pxgShader* pxgShaderLib::InitTerrainShader()
{
    const char* vs_source =
    "#version 130\n"
    SHADER
    (
            in vec3 position;
           // in vec2 texCoord;
           // in vec3 normal;
            out vec4 TexCoord;
            out float vDepth;
          //  out vec3 N;
            uniform mat4 model;
            uniform mat4 VP;
            uniform sampler2D heightMap;

            uniform vec2 dim;
            uniform vec2 offset;
            uniform float tiling;
            uniform float height;

            void main()
            {
                vec2 p = vec2(position.xz)+offset;
                gl_Position = VP*model*vec4(p.x,height*texture(heightMap,vec2(p.x/dim.x,p.y/dim.y)).r,p.y,1);
                TexCoord.xy = vec2(p.x/dim.x,p.y/dim.y)*tiling;
                TexCoord.zw = vec2(p.x/dim.x,p.y/dim.y);
                vDepth = gl_Position.z;
            }
    );

    const char* fs_source =
    "#version 130\n"
    "#extension GL_EXT_texture_array : enable\n"
    SHADER
    (
        in vec4 TexCoord;
        in float vDepth;
        out vec4 outColor;
        uniform sampler2DArray tex;
        uniform sampler2DArray texAlpha;
        uniform int numTextures;

        const vec4 fogC = vec4(0.5,0.5,0.5,1);

        void main()
        {
            if(numTextures<=0)
            {
                outColor = vec4(0,0,0,1);
                return;
            }
            outColor = texture2DArray(tex,vec3(TexCoord.xy, 0));//texture2DArray(tex,vec3(TexCoord.xy, 0));//texture2DArray(tex,vec3(TexCoord.xy, 0));
            for(int i = 1;i<numTextures;i++)
            {
                outColor = mix(texture2DArray(tex,vec3(TexCoord.xy, i)),outColor,1-texture2DArray(texAlpha,vec3(TexCoord.zw, i-1)).r);
            }
            outColor = mix(outColor,fogC,vDepth/2000);
        }
    );

    pxgShader* shader = new pxgShader;
    shader->VS(&vs_source);
    shader->FS(&fs_source);
    std::vector<std::string> attribs;
    attribs.push_back("position");
    attribs.push_back("texCoord");
    shader->Link(PXG_VERTEX3D,attribs);
    shader->AddUniform("model", PXG_4FM);
    shader->SetUniformTarget("model",PXG_MODEL);
    shader->AddUniform("VP", PXG_4FM);
    shader->SetUniformTarget("VP",PXG_VIEW_PROJECTION);
    shader->AddUniform("tiling",PXG_1F);
    shader->AddUniform("dim",PXG_2F);
    shader->AddUniform("offset",PXG_2F);
    shader->AddUniform("numTextures",PXG_1I);
    shader->AddTexture(PXG_TEXTURE0, "heightMap");
    shader->AddTexture(PXG_TEXTURE1, "tex");
    shader->AddTexture(PXG_TEXTURE2, "texAlpha");
    shader->AddUniform("height", PXG_1F);
    return shader;
}
