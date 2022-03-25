#ifndef _TOTEM_FONT_RENDERER_H_
#define _TOTEM_FONT_RENDERER_H_

#include "Math/Vec.h"
#include "ResourceManager.h"
#include "Texture.h"

namespace totem
{
   class Renderer;

   class Font : public Resource
   {
      friend class FontRenderer;
   public:
      Font(const char* fontPath);
      ~Font();

      virtual void Load() override;

      virtual ResourceType GetType() const override 
      { return ResourceType::Font; }

      static ResourceType GetStaticType()
      { return ResourceType::Font; }
      
   private:
      struct Character
      {
         math::vec2f size;
         math::vec2f bearing;
         float advance;
         Texture* texture;
         Character(const math::vec2f& size, const math::vec2f& bearing,
                  float advance, Texture* texture);
         ~Character();
      };
 
      enum { MaxCodepoint = 12288 };
      // Most common codepoints (0x02ff)
      Character* m_Characters[MaxCodepoint];
   };

   class FontRenderer
   {
   public:
      FontRenderer(Renderer* master);
      void SetFont(const char* fontPath);

      void DrawChar(unsigned int codepoint, const math::vec2f& pos,
                  float scale, const math::vec4f& color);
      float GetAdvance(unsigned int codepoint, float scale) const;
      float GetHeight(unsigned int codepoint, float scale) const;
      void SetAspectRatio(float aspectRatio);
   private:
      Font* m_CurrentFont;
      Renderer* m_Master;
      const char* m_FontShaderId;
   };

}

#endif

