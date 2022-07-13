#include <glad/glad.h>
#include <string>

#include "Renderer.h"
#include "Assert.h"

namespace totem
{
   bool Renderer::s_OpenGLInitialized = false;

   //From learnopengl.com/In-Practice/Debugging
   GLenum glCheckError_(const char *file, int line)
   {
      GLenum errorCode;
      while ((errorCode = glGetError()) != GL_NO_ERROR)
      {
         std::string error;
         switch (errorCode)
         {
            case GL_INVALID_ENUM:
               error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:
               error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:
               error = "INVALID_OPERATION"; break;
            case GL_OUT_OF_MEMORY:
               error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
               error = "INVALID_FRAMEBUFFER_OPERATION"; break;

         }
         std::cout << error << " | " << file << 
               " (" << line << ")" << std::endl;
      }
      return errorCode;
   }

   #define glCheckError() glCheckError_(__FILE__, __LINE__) 

   Renderer::Renderer(Window *window, ResourceManager* resManager) 
      : m_Window(window), m_ShaderManager(resManager), m_CanvasScale(1, 1)
   {
      // There should be a valid context for glad to initialize 
      m_Window->MakeCurrent();
      if(!s_OpenGLInitialized)
      { 
         if (!gladLoadGLLoader((GLADloadproc)
                  m_Window->GetOpenGLLoaderFunc()))
         {
            TOTEM_ASSERT(false, "Glad Failed to initialize!");
            return;
         }
      }

      s_OpenGLInitialized = true;
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

      glGenVertexArrays(1, &m_VAO);
      glBindVertexArray(m_VAO);

      static float vertices[] =
      {
         -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
          1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
          1.0f,  1.0f, 0.0f,     1.0f, 1.0f,
         -1.0f,  1.0f, 0.0f,     0.0f, 1.0f
      };

      static unsigned int indices[] = 
      {
         0, 1, 2,
         2, 3, 0
      };

      glGenBuffers(1, &m_VBO);
      glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                     vertices, GL_STATIC_DRAW);
      glGenBuffers(1, &m_EBO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
                     indices, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, 
                  GL_FALSE, 5 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, 2, GL_FLOAT,
                  GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);

      uint32_t whiteTexData = 0xffffffff; 
      m_WhiteTexture = 
         new Texture((unsigned char*)&whiteTexData, 1, 1, 4);

      m_DefaultTextureShader = 
         m_ShaderManager->Get<Shader>("resources/shaders/DefTexture.glsl");
      m_DefaultFontShader = 
         m_ShaderManager->Get<Shader>("resources/shaders/DefFont.glsl");

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   }

   Renderer::~Renderer()
   {
      delete m_WhiteTexture;
   }
 
   void Renderer::Clear(const math::vec4f& color)
   {
      glClearColor(color.x, color.y, color.z, color.w);
      glClear(GL_COLOR_BUFFER_BIT);
   }

   void Renderer::DrawRect(const Rect& rect)
   {
      DrawRect(rect, *m_WhiteTexture, *m_DefaultTextureShader);
   }

   void Renderer::DrawRect(const Rect& rect, const Texture& texture)
   {
      DrawRect(rect, texture, *m_DefaultTextureShader);
   }

   void Renderer::DrawRect(const Rect& rect, const Texture& texture,
                           Shader& shader)
   {
      shader.Use();

      const float eps = 0.0001f;
      const math::vec2f& pos = rect.GetPos();
      const math::vec2f& scale = rect.GetScale();
      const math::vec2f& rotationAxis = rect.GetRotationAxis();

      math::mat4f mat;

      // if all rotation parameters are almost default ones (zeroes)
      // then just draw not rotated rect,
      // otherwise draw rotated one
      if(abs(rect.GetRotationAngle()) <= eps &&
         abs(rotationAxis.x) <= eps && abs(rotationAxis.y) <= eps)
      {
         // Draw not rotated rect
         mat = math::getTranslate(pos.x, pos.y)
                     *  math::getScale(scale.x, scale.y);
      }
      else
      {
         // Draw rotated rect
         mat = 
            math::getTranslate(pos.x, pos.y)
            *  math::getRotationZ(math::degToRad(rect.GetRotationAngle()))
            *  math::getTranslate(-rotationAxis.x * scale.x, 
                                    -rotationAxis.y * scale.y)
            *  math::getScale(scale.x, scale.y);
      }

      shader.SetUniformMatrix4fv("vModelMat", mat);
      shader.SetUniform4f("fColor", rect.GetColor());
 
      texture.Bind();
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   }
 
   void Renderer::DrawCharacter(unicode_t codepoint, const math::vec2f& pos,
                                 float size, const math::vec4f& color,
                                 const Font& font)
   {
      if(!font.IsCharAvailable(codepoint))
      {
         LOG_ERROR("Unhandled codepoint %d in font %s", codepoint,
                     font.GetName());
         return;
      }

      // Scale em square by user specified size.
      const math::vec2f em = { GetEM() * size, GetEM() * size };

      // Convert all font data to canvas coords.

      math::vec2f charScale =
      {
         em.x * font.GetSize(codepoint).x,
         em.y * font.GetSize(codepoint).y
      };
 
      math::vec2f charBearing = 
      {
         2 * em.x * font.GetBearing(codepoint).x,
         2 * em.y * font.GetBearing(codepoint).y
      };

      math::vec2f charPos = 
      {
         pos.x + charBearing.x + charScale.x,
         pos.y + charScale.y - (2 * charScale.y - charBearing.y)
      };

      Rect rect;
      rect.SetPos(charPos)
         .SetScale(charScale)
         .SetColor(color);

      DrawRect(rect, *(font.GetTexture(codepoint)), *m_DefaultFontShader);  
   }

   void Renderer::DrawText(const Text& text, const math::vec2f& pos,
                           const Font& font, float size,
                           const math::vec4f& color)
   {
      // Scale em square by user specified size.
      const math::vec2f em = { GetEM() * size, GetEM() * size };

      math::vec2f currPos = pos;
      Text::Iterator iter(text);
      while(!iter.HasEnded()) 
      {
         DrawCharacter(iter.Get(), currPos, size, color, font);
         currPos.x += font.GetAdvance(iter.Get()) * (2 * em.x);
         iter.Next();
      }
   }

   void Renderer::DrawAlignedText(const Text& text, 
                                  const math::vec2f& boxPos,
                                  const math::vec2f& boxScale,
                                  const Font& font,
                                  float size,
                                  const math::vec4f& color,
                                  int alignFlags)
   {
      math::vec2f pos;
      math::vec2f textSize = CalcBBox(text, size, font);

      math::vec2f leftTop(
         boxPos.x - boxScale.x,
         boxPos.y + boxScale.y
      );

      math::vec2f rightBottom(
         boxPos.x + boxScale.x,
         boxPos.y - boxScale.y
      );

      if(alignFlags & TextAlign::HCenter)
      {
         pos.x = (leftTop.x + rightBottom.x) / 2;
         pos.x -= textSize.x / 2;
      }

      if(alignFlags & TextAlign::VCenter)
      {
         pos.y = (leftTop.y + rightBottom.y) / 2;
         pos.y -= textSize.y / 2;
      }

      DrawText(text, pos, font, size, color);
   }
 
   math::vec2f Renderer::CalcBBox(const Text& text, float size,
                                 const Font& font) const
   {
      // Scale em square by user specified size.
      const math::vec2f em = { GetEM() * size, GetEM() * size };

      math::vec2f res { 0, 0 };
      Text::Iterator iter(text);
      while(!iter.HasEnded())
      {
         res.x += font.GetAdvance(iter.Get()) * em.x;
         float h = font.GetSize(iter.Get()).y * em.y;
         res.y = res.y < h ? h : res.y;
         iter.Next();
      }
      return res;
   }


   float Renderer::GetEM() const
   {
      return m_CanvasScale.y / 20;
   }

   void Renderer::SetCanvasScale(const math::vec2f& scale)
   {
      math::mat4f mat = math::getOrthoProj(scale.x, scale.y, -1.0f, 1.0f);

      m_DefaultTextureShader->Use();
      m_DefaultTextureShader->SetUniformMatrix4fv("vProjMat", mat);

      m_DefaultFontShader->Use();
      m_DefaultFontShader->SetUniformMatrix4fv("vProjMat", mat);

      m_CanvasScale = scale;
   }

   const math::vec2f& Renderer::GetCanvasScale() const
   {
      return m_CanvasScale;
   }

   void Renderer::SetViewport(unsigned int width, unsigned int height)
   {
      glViewport(0, 0, width, height);
   } 
}
