/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXG_HUD_MANAGER
#define PXG_HUD_MANAGER


#include <vector>
#include <string>
#include <map>

#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

typedef unsigned int GLuint;

struct pxgVertex2dT;

class pxgHUDObject;
class pxgShader;
class pxgBuffer;

struct _font
{
    _font();
    ~_font();
    int numGlyphs;
    GLuint* glyphs;
    pxgVertex2dT** glyphsVertices;
};

class pxgHUDManager
{
public:
    pxgHUDManager();

    bool Init(float width, float height);
    void Reshape(float width, float height);
    void AddHUD(pxgHUDObject* hud);
    bool LoadFont(std::string name, float size);
    void Render();
    void Update();
    void Translate(glm::vec2 t) { translationVector = t; }
    void Rotate(float angle) { rotation = angle; }
    void Scale(float scale) { scaling = scale; }
    glm::vec2 GetTranslation() const { return translationVector; }
    float GetRoation() const { return rotation; }
    float GetScaling() const { return scaling; }

private:
    std::vector<pxgHUDObject*> huds;
    float rotation;
    float scaling;
    glm::vec2 translationVector;
    glm::mat4 transformation;
    glm::mat4 ortho;

    pxgShader* shader;
    pxgShader* fontShader;
    pxgBuffer* squareVerts;
    pxgBuffer* squareIndices;

    GLuint vertexArray;
    GLuint* glyphs;
    GLuint glyphVB;
    GLuint glyphsVAO;
    pxgVertex2dT** glyphsVertices;
    //pxgVertex2dT* textureVertices;
    int numGlyphs;
    glm::vec4* glyphsMetrics;

    float width, height;

    void UpdateTransformation();
    void RenderGlyph(void* face, char c, GLuint texture, pxgVertex2dT** glyphVertices);
    void DrawText(pxgHUDObject* obj);
    void RenderText(pxgHUDObject* obj, void* face);
    void InitFonts();

    std::map<std::string, _font*> fonts;
    _font *currentFont;

};

#endif //PXG_HUD_MANAGER
