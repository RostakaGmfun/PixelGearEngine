/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgTerrain.h>
#include <pxgShader.h>
#include <pxgTexture.h>
#include <pxgBuffer.h>
#include <pxgShaderLib.h>
#include <pxgWorld.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <pxgTools.h>

pxgTerrain::pxgTerrain(): shader(0), vbo(0), ibo(0), vao(0), diffuse(0), alpha(0), normal(0), height(0), tiling(1,1), numTiles(1), world(0), terrainHeight(1), heightData(NULL)
{
    transformation = glm::mat4(1.0f);
    scaling = glm::vec3(1.0f);
    translation = rotation = glm::vec3(0.0f);
}

bool pxgTerrain::Init(int resolution, float sc, int numLods, int tiles)
{
    if(resolution<2)
    {
        PXG::Log("pxgTerrain::Init() error: resolution less than 2", ERR);
        return false;
    }

    if(sc<=0)
    {
        return false;
    }
    scale = sc;

   /* if(tiles<1)
    {
        PXG::Log("pxgTerrain::Init() error: tile count less than 1", ERR);
        return false;
    }*/

    numTiles = tiles;
    res = resolution;

   /* PXG::glGenVertexArrays(1,&vao);

    if(!vao)
    {
        PXG::Log("pxgTerrain::Init(): error generating vertex array", ERR);
        return false;
    }*/

   /* PXG::glBindVertexArray(vao);
    vbo = new pxgBuffer;
    ibo = new pxgBuffer;
    vbo->Init();
    ibo->Init();
   // vbo->Use();
  //  ibo->Use();

    pxgVertex3dT *verts = new pxgVertex3dT[(resolution)*(resolution)];

    int count = 0;
    int width = resolution/2;

    using namespace glm;

    for(int i = 0;i<resolution;i++)
        for(int j = 0;j<resolution;j++)
        {
            verts[count].pos = vec3(i,0,j);
            verts[count++].tc = vec2(float(i)/resolution,float(j)/resolution);
        }
    vbo->SetData(VERTEX,(void*)verts,sizeof(pxgVertex3dT)*resolution*resolution,resolution*resolution);

    GLuint *indices = new GLuint[(resolution-1)*(resolution-1)*6];

    int vertexCount = count;


    count = 0;
    for(int i = 0;i<resolution-1;i++)
    {
        for(int j = 0;j<resolution-1;j++)
        {
            indices[count++] = i*resolution+j;
            indices[count++] = i*resolution+j+1;
            indices[count++] = (i+1)*resolution+j;
            indices[count++] = i*resolution+j+1;
            indices[count++] = (i+1)*resolution+j+1;
            indices[count++] = (i+1)*resolution+j;
        }
    }
    int indexCount = count;

    ibo->SetData(ELEMENT,(void*)indices,sizeof(GLuint)*(resolution-1)*(resolution-1)*6,(resolution-1)*(resolution-1)*6);

    PXG::glVertexAttribPointer(PXG_POSITION, sizeof(glm::vec3)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex3dT), (void*)offsetof(pxgVertex3dT,pos));
    PXG::glEnableVertexAttribArray(PXG_POSITION);
    PXG::glVertexAttribPointer(PXG_TEXCOORD,sizeof(glm::vec2)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex3dT), (void*)offsetof(pxgVertex3dT, tc));
    PXG::glEnableVertexAttribArray(PXG_TEXCOORD);

    PXG::glBindVertexArray(0);*/

//    PXG::Log("pxgTerrain::Init(): successfully initialised terrain grid with resolution "+std::to_string(resolution)+", vertex count "+std::to_string(vertexCount)+", indexCount "+std::to_string(indexCount));

    pxgShaderLib* sl = new pxgShaderLib;
    shader = sl->InitTerrainShader();
    if(!shader)
    {
        PXG::Log("pxgTerrain::Init(): error creating shader", ERR);
    }

    for(int i = 0;i<numLods;i++)
    {
        int r = resolution/(i+1);
        float s = float(resolution)/float(r);
        PXG::Log(std::to_string(s));
        pxgTerrainLOD* lod = InitLOD(r,s*scale);
        lodData.push_back(lod);
    }

    return true;
}

bool pxgTerrain::Render()
{
    if(!world) return false;
    if(!shader) return false;
    if(!height) return false;
    if(!diffuse) return false;
    if(!alpha) return false;
    if(!shader->Use()) return false;

    //bind height map
    shader->SetTexture(PXG_TEXTURE0,height);

    //bind diffuse maps array
    shader->SetTexture(PXG_TEXTURE1, diffuse);

    //bind alpha maps array
    shader->SetTexture(PXG_TEXTURE2, alpha);

    //bind normal maps array
    if(normal)
    {
        shader->SetTexture(PXG_TEXTURE3, normal);
    }

    shader->SetUniform(PXG_VIEW,glm::value_ptr(world->GetView()));
    shader->SetUniform(PXG_PROJECTION,glm::value_ptr(world->GetProj()));
    shader->SetUniform(PXG_VIEW_PROJECTION,glm::value_ptr(world->GetVP()));
    shader->SetUniform(PXG_MODEL, glm::value_ptr(transformation));
    shader->SetUniform("dim",glm::value_ptr(glm::vec2(lodData[0]->width*numTiles)));
    int nt = 3;
    shader->SetUniform("numTextures",&nt);
    glm::vec2 p(translation.x,translation.z);
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    int current = 0;
    for(int i = 0;i<numTiles;i++)
    {
        for(int j = 0;j<numTiles;j++)
        {

            p.x += (float(res))*scale;
            shader->SetUniform("offset",glm::value_ptr(glm::vec2(p.x,p.y)));
            float dist = glm::abs(glm::distance(glm::vec2(p.x+res/2,p.y+res/2),glm::vec2(world->GetCamera()->GetTranslation().x,world->GetCamera()->GetTranslation().z)));
            float d2 = world->GetCamera()->GetFar()+res*1.1f;
            if(dist>d2)
                continue;
            float d = dist/d2;
            int lod = glm::floor(d*lodData.size());///lodData.size();
            if(lod>=lodData.size())
                lod = lodData.size()-1;
            current = lod;
            PXG::glBindVertexArray(lodData[lod]->vao);
            glDrawElements(GL_TRIANGLES,lodData[lod]->ibo->GetSize(),GL_UNSIGNED_INT,0);
            PXG::glBindVertexArray(lodData[lodData.size()-1]->vao);
            glDrawElements(GL_TRIANGLES,lodData[lodData.size()-1]->ibo->GetSize(),GL_UNSIGNED_INT,0);


        }
        p.y += (float(res))*scale;
        p.x = 0;
    }
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    return true;
}

void pxgTerrain::Destroy()
{
    return;
}

void pxgTerrain::Update()
{
    UpdateTransformation();
    return;
}

void pxgTerrain::SetHeight(float h)
{
    terrainHeight = h;
    if(shader)
        shader->SetUniform("height",&terrainHeight);
        return;
}

void pxgTerrain::SetTextureTiling(glm::vec2 t)
{
    tiling = t;
    if(shader)
        shader->SetUniform("tiling",&tiling);
    return;
}

void pxgTerrain::LoadHeightData()
{
    if(heightData)
        return;
    if(!height)
        return;
    glGetTexImage(height->GetName(),0,GL_R,GL_UNSIGNED_BYTE,heightData);
}

float pxgTerrain::GetHeightAtPos(glm::vec2 p)
{
    if(!heightData)
        return 0;

    using namespace glm;

    /*
    A________________B
    |               |
    |               |
    |               |
    |               |
   C|_______________|D

    */

    vec2 t(translation.x,translation.z);
    vec2 a = vec2(0)+t;
    vec2 b = vec2(translation.x*scaling.x,0)+t;
    vec2 c = vec2(0,translation.z*scaling.z)+t;
    vec2 d = vec2(translation.x*scaling.x,translation.z*scaling.z)+t;
    if(p.x<a.x||p.y<a.y||p.x>d.x||p.y)
        return 0; //outside terrain plane
    //TODO

}

pxgTerrainLOD* pxgTerrain::InitLOD(int r, float scale)
{
    if(scale==0||r<2)
        return NULL;
    int resolution = r+1;
    pxgTerrainLOD* lod = new pxgTerrainLOD;
    lod->resolution = resolution;
    lod->width = scale*float(resolution-2);
    lod->vbo = new pxgBuffer;
    lod->ibo = new pxgBuffer;
    lod->vbo->Init();
    lod->ibo->Init();

    pxgVertex3d *verts = new pxgVertex3d[(resolution)*(resolution)];

    int count = 0;
    int width = resolution/2;

    using namespace glm;

    for(int i = 0;i<resolution;i++)
        for(int j = 0;j<resolution;j++)
        {
            verts[count++].pos = vec3(i*scale,0,j*scale);
            //verts[count++].tc = vec2(float(i)/resolution,float(j)/resolution);
        }
    lod->vbo->SetData(VERTEX,(void*)verts,sizeof(pxgVertex3d)*resolution*resolution,resolution*resolution);

    GLuint *indices = new GLuint[(resolution-1)*(resolution-1)*6];

    int vertexCount = count;


    count = 0;
    for(int i = 0;i<resolution-1;i++)
    {
        for(int j = 0;j<resolution-1;j++)
        {
            indices[count++] = i*resolution+j;
            indices[count++] = i*resolution+j+1;
            indices[count++] = (i+1)*resolution+j;
            indices[count++] = i*resolution+j+1;
            indices[count++] = (i+1)*resolution+j+1;
            indices[count++] = (i+1)*resolution+j;
        }
    }
    int indexCount = count;

    lod->ibo->SetData(ELEMENT,(void*)indices,sizeof(GLuint)*(resolution-1)*(resolution-1)*6,(resolution-1)*(resolution-1)*6);

    PXG::glGenVertexArrays(1,&lod->vao);
    PXG::glBindVertexArray(lod->vao);

    lod->vbo->Use();
    lod->ibo->Use();

    PXG::glVertexAttribPointer(PXG_POSITION, sizeof(glm::vec3)/4, GL_FLOAT, GL_FALSE, 0, 0);
    PXG::glEnableVertexAttribArray(PXG_POSITION);
  //  PXG::glVertexAttribPointer(PXG_TEXCOORD,sizeof(glm::vec2)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex3dT), (void*)offsetof(pxgVertex3dT, tc));
  //  PXG::glEnableVertexAttribArray(PXG_TEXCOORD);

    PXG::glBindVertexArray(0);
    PXG::Log(std::to_string(vertexCount));

    return lod;
}
