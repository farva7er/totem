#include <ft2build.h>
#include <freetype/freetype.h>

#include "Renderer.h"

namespace totem
{
   Font::Character::Character(math::vec2i size, math::vec2i bearing,
                        unsigned int advance, Texture* texture)
      : size(size), bearing(bearing), advance(advance), texture(texture)
   {}

   Font::Character::~Character()
   {
      delete texture;
   }

   Font::Font(const char* fontPath)
      : Resource(fontPath)
   {
      for(int i = 0; i < Max_Codepoint; i++)
      {
         m_Characters[i] = nullptr;
      }
   }

   Font::~Font()
   { 
      for(int i = 0; i < Max_Codepoint; i++)
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

      error = FT_Set_Char_Size(face, 0, 32*64, 140, 140);
      if(error)
      {
         LOG_ERROR("Couldn't set char size for font %s", m_ResourceId);
         FT_Done_Face(face);
         FT_Done_FreeType(ft);
         return;
      }
      FT_ULong  charcode;
      FT_UInt   gindex;


      charcode = FT_Get_First_Char(face, &gindex);
      while (gindex != 0 && charcode < Max_Codepoint)
      {
         error = FT_Load_Char(face, charcode, FT_LOAD_RENDER);
         if(error)
         {
            LOG_ERROR("Couldn't load glyph for font %s", m_ResourceId);
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
         charcode = FT_Get_Next_Char(face, charcode, &gindex);
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

   void FontRenderer::DrawChar(unsigned int codepoint, float x, float y,
                              float scale, math::vec4f color)
   {
      if(!m_CurrentFont)
      {
         LOG_ERROR("Font is not set!");
         return;
      }

      Font::Character* ch = m_CurrentFont->m_Characters[codepoint];
      if(!ch)
      {
         LOG_ERROR("Unhandled codepoin in font %s", m_CurrentFont->GetId());
         return;
      }
      

      float xpos = x + (m_Master->PixelUnitXToNormal(ch->bearing.x) +
                        m_Master->PixelUnitXToNormal(ch->size.x)/2) * scale;

      float dy = (m_Master->PixelUnitYToNormal(ch->size.y)/2 -
                  m_Master->PixelUnitYToNormal(ch->size.y - ch->bearing.y))
                  * scale;
      float ypos = y - dy;

      float w = m_Master->PixelUnitXToCam(ch->size.x) * scale;
      float h = m_Master->PixelUnitYToCam(ch->size.y) * scale;
      LOG_INFO("h: %f, dy: %f",h, dy);

      m_Master->DrawRect(math::vec2f(xpos, ypos),
                        math::vec2f(w, h),
                        ch->texture,
                        color,
                        m_FontShaderId);
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
