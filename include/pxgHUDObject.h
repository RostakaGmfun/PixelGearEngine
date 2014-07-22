/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#ifndef PXG_HUD_Object
#define PXG_HUD_Object

#include <pxgNode.h>
#include <pxgCallback.h>

#include <string>

#include <glm/vec2.hpp>

class pxgHUDManager;
class pxgTexture;

enum PXG_HUD_TYPE
{
    PXG_HUD_TEXTURE,
    PXG_HUD_TEXT
};

enum PXG_HUDSTYLE_TYPE
{
    PXG_HUDSTYLE_NORMAL,
    PXG_HUDSTYLE_HOVERED,
    PXG_HUDSTYLE_CLICKED
};

class pxgHUDObject
{
public:
    pxgHUDObject();
    bool Render();
    void Update();

    void SetTexture(pxgTexture* tex) { texture = tex; type = PXG_HUD_TEXTURE; }
    void SetVisible(bool v) { visible = v; }
    bool IsVisible() const { return visible; }
    void Translate(glm::vec2 t) { translationVector = t; }
    void Rotate(float angle) { rotation = angle; }
    void SetSize(glm::vec2 s) { size = s; }
    glm::vec2 GetTranslation() const { return translationVector; }
    glm::mat4 GetTransformation() const { return transformation; }
    //void Transform(glm::mat3 t) { m_transform = t; }
    float GetRotation() const { return rotation; }
    glm::vec2 GetSize() const { return size; }
    void SetText(std::string t) { text = t; type = PXG_HUD_TEXT; }
    void SetColor(glm::vec4 c) { color = c; }
    const glm::vec4 GetColor() const { return color; }
    std::string GetText() const { return text; }
    PXG_HUD_TYPE GetType() const { return type; }
    void SetFont(std::string fontName) { font = fontName; }
    std::string GetFont() const { return font; }

    void NormalStyleCallback(pxgCallback<void, pxgHUDObject*, void*>* cb);
    void HoveredStyleCallback(pxgCallback<void, pxgHUDObject*, void*>* cb);
    void ClickedStyleCallback(pxgCallback<void, pxgHUDObject*, void*>* cb);

private:
    float rotation;
    glm::vec2 size;
    glm::vec2 translationVector;
    glm::vec4 color;
    glm::mat4 transformation;
    pxgTexture* texture;
    std::string text;
    PXG_HUD_TYPE type;
    std::string font;
    bool visible;

    PXG_HUDSTYLE_TYPE hudStyle;
};

#endif //PXG_HUD_Object
