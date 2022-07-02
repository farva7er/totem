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

   Font::Font(const char* name, ResourceManager* manager)
      : Resource(name, manager)
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

      error = FT_New_Face(ft, GetName(), 0, &face);
      if(error)
      {
         LOG_ERROR("Couldn't load font %s", GetName());
         FT_Done_Face(face);
         FT_Done_FreeType(ft);
         return;
      }

      int pixelSize = 26;
      error = FT_Set_Pixel_Sizes(face, 0, pixelSize);
      if(error)
      {
         LOG_ERROR("Couldn't set char size for font %s", GetName());
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
         if(error)
         {
            LOG_ERROR("Couldn't load glyph for font %s", GetName());
         } 

         if(!error)
         {
            error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
            if(error)
            {
               LOG_ERROR("Couldn't render for font %s Error code: %d,\
                           charcode: %d", GetName(), error, charcode);
            }
         }
 
         m_Characters[charcode] = 
            new Character
            (
                  math::vec2f(face->glyph->bitmap.width / (float)pixelSize,
                              face->glyph->bitmap.rows / (float)pixelSize),
                  math::vec2f(face->glyph->bitmap_left / (float)pixelSize,
                              face->glyph->bitmap_top / (float)pixelSize),
                  (face->glyph->advance.x >> 6) / (float)pixelSize,
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

   bool Font::IsCharAvailable(unicode_t codepoint) const
   {
      return m_Characters[codepoint] != nullptr;
   }

   const math::vec2f& Font::GetSize(unicode_t codepoint) const
   {
      TOTEM_ASSERT(m_Characters[codepoint],
            "Unhandled codepoint %d in font %s", codepoint, GetName());
      return m_Characters[codepoint]->size;
   }

   const math::vec2f& Font::GetBearing(unicode_t codepoint) const
   {
      TOTEM_ASSERT(m_Characters[codepoint],
            "Unhandled codepoint %d in font %s", codepoint, GetName());
      return m_Characters[codepoint]->bearing;
   }

   float Font::GetAdvance(unicode_t codepoint) const
   {
      TOTEM_ASSERT(m_Characters[codepoint],
            "Unhandled codepoint %d in font %s", codepoint, GetName());
      return m_Characters[codepoint]->advance;
   }

   const Texture* Font::GetTexture(unicode_t codepoint) const
   {
      TOTEM_ASSERT(m_Characters[codepoint],
            "Unhandled codepoint %d in font %s", codepoint, GetName());
      return m_Characters[codepoint]->texture;
   }   
}
