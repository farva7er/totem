#ifndef _TOTEM_RENDERER_H_
#define _TOTEM_RENDERER_H_

#include "Window.h"
#include "Math/Mat.h"
#include "Math/Vec.h"
#include "Texture.h"
#include "Shader.h"
#include "FontRenderer.h"
#include "Renderer/RendererPrimitives/Rect.h"

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
      Renderer(Window *window);
      virtual ~Renderer();

      void Clear(float r, float g, float b, float a = 1.0f);

      void DrawRect(const Rect& rect);
      
      void DrawImage(const char* imagePath, const math::vec2f& pos,
                     float scale = 1.0f,
                     const math::vec4f& tintColor =
                     math::vec4f(1.0f, 1.0f, 1.0f, 1.0f));

      void DrawBackground(const char* imagePath);

      void DrawText(const char* str, const math::vec2f& pos,
                     float scale = 1.0f, const math::vec4f& color =
                     math::vec4f(1, 1, 1, 1)
                     );

      void DrawControlledText(const char* str,
                              const math::vec2f& boxPos,
                              const math::vec2f& boxScale,
                              float scale = 1.0f,
                              const math::vec4f& color = 
                              math::vec4f(1, 1, 1, 1),
                              int alignFlags =
                              TextAlign::VCenter | TextAlign::HCenter
                              );
  
      Shader* GetShader(const char* shaderId) const;
      void SetViewport(unsigned int width, unsigned int height);
      void SetCanvasScale(const math::vec2f& scale);
      const math::vec2f& GetCanvasScale() const;
   private:
      static bool s_OpenGLInitialized; 
      static const char* s_TextureShaderId;

      math::vec2f CalcTextSize(const char* str, float scale) const;
   private:
      Window* m_Window;
      unsigned int m_VBO, m_VAO, m_EBO;
      Texture* m_WhiteTexture;
      math::mat4f m_ProjMat;
      math::vec2f m_CanvasScale;
      FontRenderer m_FontRenderer;
   };
}

#endif
