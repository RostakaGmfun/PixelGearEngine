/*
 *  Pixel Gear Engine
 *  Copyright (c) 2014 Rostyslav Kurylo aka RostakaGmfun, rostawesomegd@gmail.com
 *  This source code is part of Pixel Gear Engine licenced under MIT licence. See LICENCE for details.
 */

#include <pxgHUDManager.h>
#include <pxgHUDObject.h>
#include <pxgShader.h>
#include <pxgShaderLib.h>
#include <pxgBuffer.h>
#include <pxgVertexLayouts.h>
#include <pxgTools.h>
#include <pxgHUDWidget.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>

#include <string>

pxgHUDManager::pxgHUDManager(): currentFont(NULL)
{
    transformation = glm::mat4(1);
}

bool pxgHUDManager::Init(float w, float h)
{
    width = w;height = h;
    PXG::glGenVertexArrays(1,&vertexArray);
    if(vertexArray==0)
    {
        PXG::Log("pxgHUDManager::Init(): error creating vertex array",ERR);
        return false;
    }
    PXG::glBindVertexArray(vertexArray);

    squareVerts = new pxgBuffer;
    squareVerts->Init();
    squareIndices = new pxgBuffer;
    squareIndices->Init();

    squareVerts->Use();
    squareIndices->Use();

    using namespace glm;
    pxgVertex2dT square[] =
    {
        { vec2(0,1), vec2(0,0)},
        { vec2(1,1), vec2(1,0)},
        { vec2(1,0), vec2(1,1)},
        { vec2(0,0), vec2(0,1)}
    };
    squareVerts->SetData(VERTEX,square,sizeof(square),sizeof(square)/sizeof(pxgVertex2dT));
    GLuint elements[] =
    {
        0, 1, 2,
        2, 3, 0
    };
    squareIndices->SetData(ELEMENT,elements,sizeof(elements),sizeof(elements)/sizeof(GLuint));

    shader = new pxgShader;
    const char* vs_source2 =
    "#version 130\n"
    SHADER
    (
        in vec2 position;
        in vec2 texCoord;

        out vec2 TexCoord;

        uniform mat4 ortho;
        uniform mat4 transformation;


        void main()
        {
            gl_Position = ortho*transformation*vec4(position,0,1);
            TexCoord = texCoord;
        }
    );
    const char* fs_source2 =
    "#version 130\n"
    SHADER
    (
        in vec2 TexCoord;
        out vec4 outColor;

        uniform sampler2D tex;
        uniform bool useTexture;
        uniform vec4 color;

        void main()
        {
            if(useTexture)
                outColor = texture(tex,TexCoord);
            else
                outColor = color;
        }
    );

    std::vector<std::string> attribs;
    attribs.push_back("position");
    attribs.push_back("texCoord");

    shader->VS(&vs_source2);
    shader->FS(&fs_source2);
    shader->Link(PXG_VERTEX2D,attribs);
    shader->AddUniform("ortho",PXG_4FM);
    shader->AddUniform("transformation", PXG_4FM);
    shader->AddUniform("useTexture", PXG_1I);
    shader->AddUniform("color", PXG_4F);

    PXG::glVertexAttribPointer(PXG_POSITION, sizeof(glm::vec2)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex2dT), (void*)offsetof(pxgVertex2dT,pos));
    PXG::glEnableVertexAttribArray(PXG_POSITION);
    PXG::glVertexAttribPointer(PXG_TEXCOORD,sizeof(glm::vec2)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex2dT), (void*)offsetof(pxgVertex2dT, tc));
    PXG::glEnableVertexAttribArray(PXG_TEXCOORD);
    PXG::glBindVertexArray(NULL);
    ortho = glm::ortho(0.f,width,height,0.f);

    fontShader = new pxgShader;
    const char* vs_source =
    "#version 130\n"
    SHADER
    (
        in vec2 position;
        in vec2 texCoord;

        out vec2 TexCoord;

        uniform mat4 ortho;
        uniform mat4 transformation;


        void main()
        {
            gl_Position = ortho*transformation*vec4(position,0,1);
            TexCoord = vec2(texCoord.x,texCoord.y);
        }
    );
    const char* fs_source =
    "#version 130\n"
    SHADER
    (
        in vec2 TexCoord;
        out vec4 outColor;

        uniform sampler2D tex;
        uniform vec4 color;

        void main()
        {
            outColor = vec4(1,1,1,texture(tex,TexCoord).r)*color;
        }
    );

    fontShader->VS(&vs_source);
    //PXG::Log(shader->GetVSLog());
    fontShader->FS(&fs_source);
    //PXG::Log(shader->GetFSLog());
    std::vector<std::string> attribs2;
    attribs2.push_back("position");
    attribs2.push_back("texCoord");
    fontShader->Link(PXG_VERTEX2D,attribs2);
    fontShader->AddUniform("ortho",PXG_4FM);
    fontShader->AddUniform("transformation", PXG_4FM);
   // fontShader->AddUniform("glyphMetrics", PXG_4F);
    fontShader->AddUniform("color", PXG_4F);
    //PXG::Log(shader->GetProgramLog());

    InitFonts();

}

void pxgHUDManager::Reshape(float width, float height)
{
    ortho = glm::ortho(0.f,width,height,0.f);
}

void pxgHUDManager::AddHUD(pxgHUDObject* hud)
{
    if(hud==NULL)
        return;
    huds.push_back(hud);
}

void pxgHUDManager::AddWidget(pxgHUDWidget* widget)
{
    if(!widget)
        return;
    widgets.push_back(widget);
}

void pxgHUDManager::Render()
{
                glDisable(GL_CULL_FACE);
                glActiveTexture(GL_TEXTURE0);
                for(int i = 0;i<huds.size();i++)
                {
                    huds[i]->Update();
                    if(!huds[i]->IsVisible())
                        continue;
                    if(huds[i]->GetType()==PXG_HUD_TEXTURE)
                    {
                        PXG::glBindVertexArray(vertexArray);
                            if(shader!=NULL)
                                if(!shader->Use())
                                    return;
                        if(squareVerts!=NULL)
                            if(!squareVerts->Use())
                                return;
                        if(squareIndices!=NULL)
                            squareIndices->Use();
                        shader->SetUniform("ortho",glm::value_ptr(ortho));
                        shader->SetUniform("transformation",glm::value_ptr(huds[i]->GetTransformation()));
                        shader->Use();
                        int ut;
                        if(huds[i]->Render())
                            ut = 1;
                        else
                            ut = 0;
                        shader->SetUniform("useTexture", &ut);
                        shader->SetUniform("color", glm::value_ptr(huds[i]->GetColor()));
                        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,NULL);

                    }
                    else
                    {
                        PXG::glBindVertexArray(glyphsVAO);
                        PXG::glBindBuffer(GL_ARRAY_BUFFER,glyphVB);
                        if(squareIndices!=NULL)
                            squareIndices->Use();
                        if(fontShader!=NULL)
                            if(!fontShader->Use())
                                return;
                        if(fonts.find(huds[i]->GetFont())!=fonts.end())
                            currentFont = fonts[huds[i]->GetFont()];
                        else
                            return;
                        fontShader->SetUniform("ortho",glm::value_ptr(ortho));
                        fontShader->SetUniform("color", glm::value_ptr(huds[i]->GetColor()));
                        DrawText(huds[i]);
                    }
                }
    glEnable(GL_CULL_FACE);
    for(int i = 0;i<widgets.size();i++)
        widgets[i]->Render();
    return;
}

void pxgHUDManager::RenderObject(pxgHUDObject* o)
{
    if(!o)
        return;
    glDisable(GL_CULL_FACE);
    glActiveTexture(GL_TEXTURE0);

    if(!o->IsVisible())
        return;
    if(o->GetType()==PXG_HUD_TEXTURE)
    {
        PXG::glBindVertexArray(vertexArray);
            if(shader!=NULL)
                if(!shader->Use())
                    return;
        if(squareVerts!=NULL)
            if(!squareVerts->Use())
                return;
        if(squareIndices!=NULL)
            squareIndices->Use();
        shader->SetUniform("ortho",glm::value_ptr(ortho));
        shader->SetUniform("transformation",glm::value_ptr(o->GetTransformation()));
        shader->Use();
        int ut;
        if(o->Render())
            ut = 1;
        else
            ut = 0;
        shader->SetUniform("useTexture", &ut);
        shader->SetUniform("color", glm::value_ptr(o->GetColor()));
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,NULL);

    }
    else
    {
        PXG::glBindVertexArray(glyphsVAO);
        PXG::glBindBuffer(GL_ARRAY_BUFFER,glyphVB);
        if(squareIndices!=NULL)
            squareIndices->Use();
        if(fontShader!=NULL)
            if(!fontShader->Use())
                return;
        if(fonts.find(o->GetFont())!=fonts.end())
            currentFont = fonts[o->GetFont()];
        else
            return;
        fontShader->SetUniform("ortho",glm::value_ptr(ortho));
        fontShader->SetUniform("color", glm::value_ptr(o->GetColor()));
        DrawText(o);
    }

    glEnable(GL_CULL_FACE);
}

void pxgHUDManager::Update()
{
    UpdateTransformation();
    for(int i = 0;i<huds.size();i++)
    {
        //huds[i]->Transform(transformation);
        huds[i]->Update();
    }
    return;
}

void pxgHUDManager::UpdateTransformation()
{
   // transformation = glm::mat3(1);
    //transformation = (glm::mat3)glm::translate((glm::mat4)transformation, translationVector);
    //transformation = glm::rotate(transformation, rotation, glm::vec3(0,0,1));
   // transformation = glm::scale(transformation, scaling);
}


bool pxgHUDManager::LoadFont(std::string fontName, float size)
{
    if(!FileExists(fontName))
    {
        PXG::Log("pxgHUDManager::LoadFont() error: font file does not exist: "+fontName,ERR);
        return false;
    }
    FT_Library lib;
    if (FT_Init_FreeType(&lib))
    {
        PXG::Log("pxgHUDmanager::LoadFont(): error initialising Freetype");
        return false;
    }
    FT_Face face;
    if (FT_New_Face( lib, fontName.c_str(), 0, &face ))
    {
        PXG::Log("pxgHUDmanager::LoadFont(): error creating new face from file: "+fontName,ERR);
        return false;
    }
    FT_Set_Char_Size(face, size*64, size*64, 128, 128);

    _font* _Font = new _font;
    _Font->glyphs = new GLuint[128];
    _Font->glyphsVertices = new pxgVertex2dT*[128];

    glGenTextures(128,_Font->glyphs);
    if(_Font->glyphs == 0)
    {
        PXG::Log("pxgHUDManager::LoadFont() error generating glyph textures",ERR);
        return false;
    }

    for(int i = 0;i<128;i++)
    {
        RenderGlyph(&face,i,_Font->glyphs[i], _Font->glyphsVertices);
    }

    //m_face = face;

    FT_Done_Face(face);

    FT_Done_FreeType(lib);

    if(fonts.find(fontName)!=fonts.end())
    {
        PXG::Log("pxgHUDManaher::LoadFont(): font "+fontName+" already exists. Replacing it with new font", WARN);
        delete fonts[fontName];
    }
    fonts[fontName] = _Font;

    PXG::Log("pxgHUDmanager::LoadFont(): loaded font "+fontName+" with size "+ std::to_string(size));

    return true;
}
_font::_font(): glyphs(NULL), glyphsVertices(NULL)
{}

_font::~_font()
{
    glDeleteTextures(128,this->glyphs);
    if(glyphsVertices)
        delete glyphsVertices;
}

inline int next_p2 ( int a )
{
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}

void pxgHUDManager::RenderGlyph(void* f, char c, GLuint texture, pxgVertex2dT** glyphVertices)
{
    FT_Face face = *(FT_Face*)f;
    /*if(FT_Load_Glyph( face, FT_Get_Char_Index( face, c), FT_LOAD_DEFAULT ))
    {
        PXG::Log("pxgHUDManager::LoadFont(): error loading glyph +"+c, ERR);
        return;
    }

    FT_Glyph glyph;
    if(FT_Get_Glyph( face->glyph, &glyph ))
    {
        PXG::Log("pxgHUDmanager::LoadFont(): error getting glyph image",ERR);
        return;
    }


  /*  FT_Vector origin;
    origin.x = 0;
    origin.y = 26.6*1000;

    glyph.

    FT_BBox  bbox;
    FT_Glyph_Get_CBox( glyph, FT_GLYPH_BBOX_PIXELS, &bbox );

    float width2  = bbox.xMax - bbox.xMin;
    float height2 = bbox.yMax - bbox.yMin;*/

   /* FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

    FT_Bitmap& bitmap=bitmap_glyph->bitmap;*/

   if(FT_Load_Char(face, c, FT_LOAD_RENDER))
   {
        PXG::Log("pxgHUDManager::LoadFont(): error loading glyph +"+c, ERR);
        return;
   }

   FT_GlyphSlot g = face->glyph;

    int width = next_p2(g->bitmap.width);
    int height = next_p2(g->bitmap.rows);

    GLubyte* expanded_data = new GLubyte[ width * height];

	//Here we fill in the data for the expanded bitmap.
	//Notice that we are using two channel bitmap (one for
	//luminocity and one for alpha), but we assign
	//both luminocity and alpha to the value that we
	//find in the FreeType bitmap.
	//We use the ?: operator so that value which we use
	//will be 0 if we are in the padding zone, and whatever
	//is the the Freetype bitmap otherwise.
	for(int j=0; j <height;j++) {
		for(int i=0; i < width; i++){
            expanded_data[(i+j*width)]= expanded_data[(i+j*width)+1] =
				(i>=g->bitmap.width || j>=g->bitmap.rows) ?
				0 : g->bitmap.buffer[i + g->bitmap.width*j];
		}
	}

	float	x=(float)g->bitmap.width / (float)width,
			y= (float)g->bitmap.rows / (float)height;



    float x2 = g->bitmap_left;
    float y2 = g->bitmap_top;
    float w = g->bitmap.width;
    float h = g->bitmap.rows;


    using namespace glm;
    glyphVertices[c] = new pxgVertex2dT[4];
    glyphVertices[c][0] = {  vec2(0,h-y2), vec2(0,y) };
    glyphVertices[c][1] = {  vec2(0,-y2), vec2(0,0) };
    glyphVertices[c][2] = {  vec2(w,-y2), vec2(x,0) };
    glyphVertices[c][3] = {  vec2(w,h-y2), vec2(x,y) };


	//glyphsMetrics[c] = glm::vec4(glyph->advance.x/6400000,glyph->advance.y/6400000,(float)bitmap.width/width, (float)bitmap.rows/height);//, (float)bitmap.width/(float)width, (float)bitmap.rows/(float)height);
	//PXG::Log(std::string(&c)+" "+std::to_string(x2));

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glBindTexture( GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height,
        0, GL_RED, GL_UNSIGNED_BYTE, expanded_data );

		delete [] expanded_data;

    return;
}

void pxgHUDManager::DrawText(pxgHUDObject* obj)
{
    if(obj==NULL)
        return;
    if(obj->GetType()!=PXG_HUD_TEXT)
        return;
    if(!currentFont)
        return;
    glm::vec2 sz = obj->GetSize();
    float rot = obj->GetRotation();
    glm::mat4 r = glm::rotate(glm::mat4(1), obj->GetRotation(),glm::vec3(0,0,1));
    glm::mat4 s = glm::scale(glm::mat4(1), glm::vec3(obj->GetSize(),1));
    std::string text = obj->GetText();
    float x = 0;
    for(int i = 0;i<text.length();i++)
    {
        if(text[i]!=' ')
        {
            glBindTexture(GL_TEXTURE_2D,currentFont->glyphs[text[i]]);
            PXG::glBufferData(GL_ARRAY_BUFFER,sizeof(pxgVertex2dT)*4,currentFont->glyphsVertices[text[i]],GL_DYNAMIC_DRAW);
            glm::mat4 transform = glm::translate(glm::mat4(1),glm::vec3(obj->GetTranslation()+glm::vec2(x,0),0));
            fontShader->SetUniform("transformation",glm::value_ptr(transform*r*s));
            glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,NULL);
        }
        else
            x+=obj->GetSize().x*currentFont->glyphsVertices[9][3].pos.x;
        x += obj->GetSize().x*currentFont->glyphsVertices[text[i]][3].pos.x+2*obj->GetSize().x;
    }
    return;
}

void pxgHUDManager::InitFonts()
{
    PXG::glGenVertexArrays(1,&glyphsVAO);
    PXG::glBindVertexArray(glyphsVAO);
    PXG::glGenBuffers(1,&glyphVB);
    PXG::glBindBuffer(GL_ARRAY_BUFFER,glyphVB);
    PXG::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareIndices->GetName());
    glyphsVertices = new pxgVertex2dT*[128];

    PXG::glVertexAttribPointer(PXG_POSITION, sizeof(glm::vec2)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex2dT), (void*)offsetof(pxgVertex2dT,pos));
    PXG::glEnableVertexAttribArray(PXG_POSITION);
    PXG::glVertexAttribPointer(PXG_TEXCOORD,sizeof(glm::vec2)/4, GL_FLOAT, GL_FALSE, sizeof(pxgVertex2dT), (void*)offsetof(pxgVertex2dT, tc));
    PXG::glEnableVertexAttribArray(PXG_TEXCOORD);
    PXG::glBindVertexArray(NULL);
}
