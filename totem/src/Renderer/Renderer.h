#ifndef _TOTEM_RENDERER_H_
#define _TOTEM_RENDERER_H_

#include "Window.h"
#include "Math/Mat.h"
#include "Math/Vec.h"
#include "Texture.h"

namespace totem
{
   class Renderer : public EventListener
   {
   public:
      Renderer(Window *window);
      virtual ~Renderer();

      void Clear(float r, float g, float b, float a = 1.0f);
      void DrawRect( math::vec2f pos, math::vec2f scale,
                     math::vec4f color);
      void DrawRect( math::vec2f pos, math::vec2f scale,
                     const char *imagePath, 
                     math::vec4f tintColor = 
                     math::vec4f(1.0f, 1.0f, 1.0f, 1.0f));
      void DrawImage(const char* imagePath, math::vec2f pos,
                     float scale = 1.0f,
                     math::vec4f tintColor =
                     math::vec4f(1.0f, 1.0f, 1.0f, 1.0f));
      void DrawBackground(const char* imagePath);

   private:
      static bool s_OpenGLInitialized;
      
      // also recalculates projection matrix
      void SetAspectRatio(float aspectRatio);
      void HandleResize(unsigned int width, unsigned int height);
      virtual void OnEvent(Event& e) override;

   private:
      Window *m_Window;
      unsigned int m_VBO, m_VAO, m_EBO, m_ShaderProgram;
      unsigned int m_WhiteTexture;
      math::mat4f m_ProjMat;
      float m_SceneSize;
      float m_AspectRatio;
   };
}

#endif
