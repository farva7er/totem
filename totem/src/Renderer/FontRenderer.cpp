#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftmodapi.h>

#include "Renderer.h"

namespace totem
{
   Font::Character::Character(const math::vec2f& size,
                              const math::vec2f& bearing,
                              float advance, Texture* texture)
      : size(size), bearing(bearing), advance(advance), texture(texture)
   {}

   Font::Character::~Character()
   {
      delete texture;
   }

   Font::Font(const char* fontPath)
      : Resource(fontPath)
   {
      for(int i = 0; i < MaxCodepoint; i++)
      {
         m_Characters[i] = nullptr;
      }
   }

   Font::~Font()
   { 
      for(int i = 0; i < MaxCodepoint; i++)
         if(m_Characters[i])
            delete m_Characters[i];
   }

   void Font::Load()
   {
      FT_Library ft;
      FT_Face face;

      int error = FT_Init_FreeType(&ft);
      if(error)
      {
         LOG_FATAL("FreeType could not initialize!");
         FT_Done_Face(face);
         FT_Done_FreeType(ft);
         return;
      }

      error = FT_New_Face(ft, m_ResourceId, 0, &face);
      if(error)
      {
         LOG_ERROR("Couldn't load font %s", m_ResourceId);
         FT_Done_Face(face);
         FT_Done_FreeType(ft);
         return;
      }


      error = FT_Set_Pixel_Sizes(face, 0, 64);
      if(error)
      {
         LOG_ERROR("Couldn't set char size for font %s", m_ResourceId);
         FT_Done_Face(face);
         FT_Done_FreeType(ft);
         return;
      }
      FT_ULong  charcode;
      FT_UInt   glyph_index;


      charcode = FT_Get_First_Char(face, &glyph_index);
      while (glyph_index != 0 && charcode < MaxCodepoint)
      {

         /* load glyph image into the slot (erase previous one) */
         error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
         if (error)
         {
            LOG_ERROR("Couldn't load glyph for font %s", m_ResourceId);
            FT_Done_Face(face);
            FT_Done_FreeType(ft);
            return;
         } 

         error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
         if (error)
         {
            LOG_ERROR("Couldn't render for font %s Error code: %d",
                        m_ResourceId, error);
            FT_Done_Face(face);
            FT_Done_FreeType(ft);
            return;
         }
 
         m_Characters[charcode] = 
            new Character
            (
                  math::vec2f(face->glyph->bitmap.width / (float)64,
                              face->glyph->bitmap.rows / (float)64),
                  math::vec2f(face->glyph->bitmap_left / (float)64,
                              face->glyph->bitmap_top / (float)64),
                  (face->glyph->advance.x >> 6) / (float)64,
                  new Texture(face->glyph->bitmap.buffer, 
                              face->glyph->bitmap.width,
                              face->glyph->bitmap.rows,
                              1)
            );
         charcode = FT_Get_Next_Char(face, charcode, &glyph_index);
      }

      FT_Done_Face(face);
      FT_Done_FreeType(ft);
   }

   FontRenderer::FontRenderer(Renderer* master)
      : m_CurrentFont(nullptr), m_Master(master),
      m_FontShaderId("fontShader")
   {
      ResourceManager& rm = ResourceManager::GetInstance();
      rm.AddResource(new Shader(
               m_FontShaderId,
               "resources/shaders/Vfont.glsl",
               "resources/shaders/Ffont.glsl"));
   }
   
   void FontRenderer::SetFont(const char *fontPath)
   {
      ResourceManager& rm = ResourceManager::GetInstance();
      Font* font = rm.GetResource<Font>(fontPath);
      if(!font)
      {
         font = rm.LoadResource<Font>(new Font(fontPath));
      }

      m_CurrentFont = font;
   }

   void FontRenderer::DrawChar(  unsigned int codepoint, 
                                 const math::vec2f& pos,
                                 float scale, const math::vec4f& color)
   {
      if(!m_CurrentFont)
      {
         LOG_ERROR("Font is not set!");
         return;
      }

      Font::Character* ch = m_CurrentFont->m_Characters[codepoint];
      if(!ch)
      {
         LOG_ERROR("Unhandled codepoint in font %s", m_CurrentFont->GetId());
         return;
      }

      math::vec2f baseScale(m_Master->GetSceneSize().y / 20, m_Master->GetSceneSize().y / 20);
      baseScale = baseScale * scale;

      math::vec2f chScale;
      chScale.x = baseScale.x * ch->size.x;
      chScale.y = baseScale.y * ch->size.y;

      math::vec2f chBearing;
      chBearing.x = 2 * baseScale.x * ch->bearing.x;
      chBearing.y = 2 * baseScale.y * ch->bearing.y;

      float xpos = pos.x + chBearing.x + chScale.x;
      float ypos = pos.y + chScale.y - (2 * chScale.y - chBearing.y);


      Rect rect = Rect::Builder()
                        .SetPos(math::vec2f(xpos, ypos))
                        .SetScale(chScale)
                        .SetTexture(ch->texture)
                        .SetColor(color)
                        .SetShaderId(m_FontShaderId)
                        .Construct();
      m_Master->DrawRect(rect); 
   
/////////////////////
/*
      float dpiScaleX = m_CurrentFont->GetDpiScale().x;
      float dpiScaleY = m_CurrentFont->GetDpiScale().y;

      float xpos = pos.x + (2 * m_Master->PixelUnitXToNormal(ch->bearing.x) +
                        m_Master->PixelUnitXToNormal(ch->size.x)) * scale
                        / dpiScaleX;

      float dy = (m_Master->PixelUnitYToNormal(ch->size.y) -
                  2 * m_Master->PixelUnitYToNormal(ch->size.y - ch->bearing.y))
                  * scale / dpiScaleY;
      float ypos = pos.y + dy;

      float w = m_Master->PixelUnitXToCam(ch->size.x) * scale / dpiScaleX;
      float h = m_Master->PixelUnitYToCam(ch->size.y) * scale / dpiScaleY;
      
      Rect rect = Rect::Builder()
                        .SetPos(math::vec2f(xpos, ypos))
                        .SetScale(math::vec2f(w, h))
                        .SetTexture(ch->texture)
                        .SetColor(color)
                        .SetShaderId(m_FontShaderId)
                        .Construct();
      m_Master->DrawRect(rect);
*/
   }

   float FontRenderer::GetAdvance(unsigned int codepoint,
                                       float scale) const
   {
      if(!m_CurrentFont)
      {
         LOG_ERROR("Font is not set!");
         return 0;
      }

      Font::Character* ch = m_CurrentFont->m_Characters[codepoint];
      if(!ch)
      {
         LOG_ERROR("Unhandled codepoint in font %s", m_CurrentFont->GetId());
         return 0;
      }

      math::vec2f baseScale(m_Master->GetSceneSize().y / 20, m_Master->GetSceneSize().y / 20);
      baseScale = baseScale * scale;

      float advance = 2 * baseScale.x * ch->advance;
      return advance;
   }

   float FontRenderer::GetHeight(unsigned int codepoint,
                                       float scale) const
   {
      if(!m_CurrentFont)
      {
         LOG_ERROR("Font is not set!");
         return 0;
      }

      Font::Character* ch = m_CurrentFont->m_Characters[codepoint];
      if(!ch)
      {
         LOG_ERROR("Unhandled codepoint in font %s", m_CurrentFont->GetId());
         return 0;
      }

      math::vec2f baseScale(m_Master->GetSceneSize().y / 20, m_Master->GetSceneSize().y / 20);
      baseScale = baseScale * scale;

      float height = 2 * baseScale.y * ch->size.y;
      return height;
   }

   void FontRenderer::SetAspectRatio(float aspectRatio)
   {
      Shader* shader = m_Master->GetShader(m_FontShaderId);
      shader->Use();
      math::vec2f sceneSize = m_Master->GetSceneSize();
      math::mat4f mat = math::getOrthoProj(sceneSize.x, 
                                           sceneSize.y, -1.0f, 1.0f);
      shader->SetUniformMatrix4fv("vProjMat", mat);
   }
}
