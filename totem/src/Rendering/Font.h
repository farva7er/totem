#ifndef _TOTEM_FONT_RENDERER_H_
#define _TOTEM_FONT_RENDERER_H_

#include "Math/Vec.h"
#include "Core/ResourceManager.h"
#include "Rendering/Texture.h"
#include "Internationalization/Text.h"

namespace totem
{
   class Font : public Resource
   {
      public:
         Font(const char* name, ResourceManager* manager);
         ~Font();

         virtual void Load() override;

         bool IsCharAvailable(unicode_t codepoint) const;
         const math::vec2f& GetSize(unicode_t codepoint) const;
         const math::vec2f& GetBearing(unicode_t codepoint) const;
         float GetAdvance(unicode_t codepoint) const;
         const Texture* GetTexture(unicode_t codepoint) const;

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
         // Most common codepoints (up to 0x02fff)
         Character* m_Characters[MaxCodepoint];
   };
}

#endif
