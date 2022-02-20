#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftmodapi.h>

#include "Renderer.h"

namespace totem
{
   Font::Character::Character(math::vec2i size, math::vec2i bearing,
                        unsigned int advance, Texture* texture)
      : size(size), bearing(bearing), advance(advance), texture(texture)
   {}
Font::Character::~Character() {
      delete texture;
   }

   Font::Font(const char* fontPath, totem::math::vec2f dpiScale)
      : Resource(fontPath), m_DpiScale(dpiScale)
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


      error = FT_Set_Char_Size(face, 0, 24*64,
                              120 * m_DpiScale.x, 120 * m_DpiScale.y);
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
                  math::vec2i(face->glyph->bitmap.width,
                              face->glyph->bitmap.rows),
                  math::vec2i(face->glyph->bitmap_left,
                              face->glyph->bitmap_top),
                  face->glyph->advance.x,
                  new Texture(nullptr,
                              face->glyph->bitmap.buffer, 
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
         font = rm.LoadResource<Font>(
               new Font(fontPath,
                     m_Master->GetContentScale()));
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

      m_Master->DrawRect(math::vec2f(xpos, ypos),
                        math::vec2f(w, h),
                        ch->texture,
                        color,
                        m_FontShaderId);
 
   }

   float FontRenderer::GetAdvanceNormal(unsigned int codepoint,
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

      float dpiScaleX = m_CurrentFont->GetDpiScale().x;
      float advance = 2 * m_Master->PixelUnitXToNormal(ch->advance >> 6) 
                  * scale / dpiScaleX;
      return advance;
   }

   float FontRenderer::GetHeightNormal(unsigned int codepoint,
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

      float dpiScaleY = m_CurrentFont->GetDpiScale().y;
      float h = 2 * m_Master->PixelUnitYToNormal(ch->size.y) 
                  * scale / dpiScaleY;
      return h;
   }

   void FontRenderer::SetAspectRatio(float aspectRatio)
   {
      Shader* shader = m_Master->GetShader(m_FontShaderId);
      shader->Use();
      float sceneSize = m_Master->GetSceneSize();
      math::mat4f mat = math::getOrthoProj(sceneSize * aspectRatio, 
                                           sceneSize, -1.0f, 1.0f);
      shader->SetUniformMatrix4fv("vProjMat", mat);
   }
}
