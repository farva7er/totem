#ifndef _TOTEM_RENDERER_H_
#define _TOTEM_RENDERER_H_

#include "Window.h"
#include "Core/ResourceManager.h"
#include "Math/Mat.h"
#include "Math/Vec.h"
#include "Texture.h"
#include "Shader.h"
#include "Font.h"
#include "Rendering/Primitives/Rect.h"
#include "Internationalization/Text.h"

namespace totem
{
   enum TextAlign
   { 
      HCenter = 0x1,
      VCenter = 0x2
   };
   
   class Renderer
   {
      public:
         Renderer(Window *window, Ref<Shader> defTextureShader,
                  Ref<Shader> defFontShader);
         virtual ~Renderer();

         void Clear(const math::vec4f& color);

         void DrawRect(const Rect& rect);
         void DrawRect(const Rect& rect, const Texture& texture);
         void DrawRect(const Rect& rect, const Texture& texture,
                        Shader& shader);

         void DrawCharacter(unicode_t codepoint, const math::vec2f& pos,
                           float size, const math::vec4f& color,
                           const Font& font);
 
         void DrawText(const Text& text, const math::vec2f& pos,
                        const Font& font, float size = 1.0f,
                        const math::vec4f& color = {1, 1, 1, 1});

         void DrawAlignedText(const Text& text,
                              const math::vec2f& boxPos,
                              const math::vec2f& boxScale,
                              const Font& font,
                              float size = 1.0f,
                              const math::vec4f& color = {1, 1, 1, 1},
                              int alignFlags =
                                 TextAlign::VCenter | TextAlign::HCenter);

         void SetViewport(unsigned int width, unsigned int height);

         const math::vec2f& GetCanvasScale() const;
         void SetCanvasScale(const math::vec2f& scale);

         float GetEM() const;

         // returns Text Bounding Box Scale.
         math::vec2f CalcBBox(const Text& text, float size,
                              const Font& font) const;
      private:
         static bool s_OpenGLInitialized; 

         Window* m_Window;
         unsigned int m_VBO, m_VAO, m_EBO;
         Texture* m_WhiteTexture;
         math::vec2f m_CanvasScale;

         Ref<Shader> m_DefaultTextureShader;
         Ref<Shader> m_DefaultFontShader;
   };
}
#endif
