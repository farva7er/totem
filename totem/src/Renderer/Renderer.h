#ifndef _TOTEM_RENDERER_H_
#define _TOTEM_RENDERER_H_

#include "Window.h"
#include "Math/Mat.h"
#include "Math/Vec.h"
#include "Texture.h"
#include "Shader.h"
#include "FontRenderer.h"
#include "RenderPrimitives.h"

namespace totem
{
   enum TextAlign
   { 
      HCenter = 0x1,
      VCenter = 0x2
   };
   
   class Renderer : public IEventListener
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
 
      float PixelUnitXToCam(int px) const;
      float PixelUnitYToCam(int py) const;
      float PixelUnitXToNormal(int px) const;
      float PixelUnitYToNormal(int py) const;

      math::vec2f ScreenToNormal(math::vec2i scrCoords) const;
      float CamUnitXToNormal(float camX) const;
      float CamUnitYToNormal(float camY) const;

      Shader* GetShader(const char* shaderId) const;
      float GetSceneSize() const;
      totem::math::vec2f GetContentScale() const;

   private:
      static bool s_OpenGLInitialized; 
      static const char* s_TextureShaderId;

      // also recalculates projection matrix
      void SetAspectRatio(float aspectRatio);
      void HandleResize(unsigned int width, unsigned int height);
      virtual void OnEvent(Event& e) override;

      math::vec2f CalcTextSize(const char* str, float scale) const;


   private:
      Window* m_Window;
      unsigned int m_VBO, m_VAO, m_EBO;
      Texture* m_WhiteTexture;
      math::mat4f m_ProjMat;
      float m_SceneSize;
      float m_AspectRatio;
      FontRenderer m_FontRenderer;
   };
}

#endif
