/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXG_TERRAIN
#define PXG_TERRAIN
#include <pxgNode.h>
#include <vector>

#include <glm/vec2.hpp>

typedef unsigned int GLuint;
typedef int GLint;

class pxgTexture;
class pxgShader;
class pxgBuffer;
class pxgWorld;

struct pxgTerrainLOD
{
    int resolution;
    float width;
    pxgBuffer* vbo;
    pxgBuffer* ibo;
    GLuint vao;
};

class pxgTerrain: public pxgNode
{
    public:
    pxgTerrain();
    bool Init(int resolution, float scale, int numLods, int numTiles);
    void SetHeightMap(pxgTexture* hmap) { height = hmap; }
    void SetHeight(float h);
    void SetDiffuseTextures(pxgTexture* diffTextures) { diffuse = diffTextures; }
    void SetAlphaTextures(pxgTexture* alphaTextures) { alpha = alphaTextures; }
    void SetNormalMaps(pxgTexture* normalMaps) { normal = normalMaps; }
    void SetTextureTiling(glm::vec2 t);
    void LoadHeightData(); /// loads height map back to RAM from GPU memory to enable GetheightAtPos()
    float GetHeightAtPos(glm::vec2 p);
    bool Render();
    void Update();
    void Destroy();
    void SetWorld(pxgWorld* w) { world = w; }
private:
    pxgShader* shader;
    pxgBuffer* vbo;
    pxgBuffer* ibo;
    GLuint vao;
    pxgTexture* diffuse;
    pxgTexture* alpha;
    pxgTexture* normal;
    pxgTexture* height;
    glm::vec2 tiling;
    int numTiles;
    float scale;
    pxgWorld* world;
    float terrainHeight;
    unsigned char* heightData;
    int res;
    std::vector<pxgTerrainLOD*> lodData;

    pxgTerrainLOD* InitLOD(int resolution, float scale);

};


#endif //PXG_TERRAIN
