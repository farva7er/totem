#include <glad/glad.h>
#include <string>

#include "Renderer.h"
#include "FontRenderer.h"
#include "Assert.h"

namespace totem
{
   bool Renderer::s_OpenGLInitialized = false;
   const char* Renderer::s_TextureShaderId = "textureShader";

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
                  error = "INVALID_ENUM"; break; case GL_INVALID_VALUE:
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

      Renderer::Renderer(Window *window) 
         : m_FontRenderer(this)
      {
         m_Window = window;

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

         ResourceManager& rm = ResourceManager::GetInstance();
         rm.AddResource(new Shader(
                  s_TextureShaderId,
                  "resources/shaders/Vtexture.glsl",
                  "resources/shaders/Ftexture.glsl"));

         glEnable(GL_BLEND);
         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

         m_FontRenderer.SetFont("resources/fonts/OpenSans-Regular.ttf");
      }

      Renderer::~Renderer()
      {
         delete m_WhiteTexture;
      }
    
      Shader* Renderer::GetShader(const char* shaderId) const
      {
         ResourceManager& rm = ResourceManager::GetInstance();
         Shader* shader = rm.GetResource<Shader>(shaderId);
         TOTEM_ASSERT(shader, "Shader is not loaded %s", shaderId);
         return shader;
      }

      void Renderer::Clear(float r, float g, float b, float a)
      {
         glClearColor(r, g, b, a);
         glClear(GL_COLOR_BUFFER_BIT);
      }
 
      
      void Renderer::DrawRect(const Rect& rect)
      {
         Shader* shader;
         const char* shaderId = 
                  rect.GetShaderId() ? rect.GetShaderId(): s_TextureShaderId;
         shader = GetShader(shaderId);
         if(!shader)
         {
            LOG_ERROR("Shader not found: %s", shaderId);
            return;
         }
         shader->Use();


         math::vec2f pos = rect.GetPos();
         math::vec2f scale = rect.GetScale();
         math::vec2f rotationAxis = rect.GetRotationAxis();
         math::mat4f mat;
         const float eps = 0.0001f; // Mehh, there are better alternatives!
                                    // But let's just use that for a while :)

         // if all rotation parameters are almost default ones (i.e. zeroes)
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

         shader->SetUniformMatrix4fv("vModelMat", mat);
         shader->SetUniform4f("fColor", rect.GetColor());

         const Texture* texture = m_WhiteTexture;
         if(rect.GetImagePath())
         {
            ResourceManager& rm = ResourceManager::GetInstance();
            texture = rm.GetResource<Texture>(rect.GetImagePath());
            if(!texture)
               texture = rm.LoadResource(new Texture(rect.GetImagePath()));
         }
         else if(rect.GetTexture())
         {
            texture = rect.GetTexture();
         }

         texture->Bind();
         glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   }

   void Renderer::DrawImage(const char* imagePath, const math::vec2f& pos,
                           float scale, const math::vec4f& tintColor)
   {
      ResourceManager& rm = ResourceManager::GetInstance();
      Texture *texture = rm.GetResource<Texture>(imagePath);
      if(!texture)
         texture = rm.LoadResource(new Texture(imagePath));
      float aspectRatio = texture->GetWidth()/(float)texture->GetHeight();

      Rect rect = Rect::Builder()
                        .SetPos(pos)
                        .SetScale(math::vec2f(scale * aspectRatio, scale))
                        .SetImagePath(imagePath)
                        .SetColor(tintColor)
                        .Construct();
      DrawRect(rect);
   }


   void Renderer::DrawBackground(const char* imagePath)
   {
      Rect rect = Rect::Builder()
                  .SetScale(m_CanvasScale)
                  .SetImagePath(imagePath)
                  .Construct();
      DrawRect(rect);
   }

   void Renderer::DrawCharacter(unicode_t codepoint, const math::vec2f& pos,
                              float scale, const math::vec4f& color)
   {
      m_FontRenderer.DrawCharacter(codepoint, pos, scale, color);
   }

   void Renderer::DrawText(const Text& text, const math::vec2f& pos,
                           float scale, const math::vec4f& color)
   {
      math::vec2f currPos = pos;
      Text::Iterator iter(text);
      while(!iter.HasEnded()) 
      {
         m_FontRenderer.DrawCharacter(iter.Get(), currPos, scale, color);
         currPos.x += m_FontRenderer.GetAdvance(iter.Get(), scale);
         iter.Next();
      }
   }

   void Renderer::DrawAlignedText(const Text& text, 
                                  const math::vec2f& boxPos,
                                  const math::vec2f& boxScale,
                                  float scale,
                                  const math::vec4f& color,
                                  int alignFlags)
   {
      math::vec2f pos;
      math::vec2f textSize = CalcBBox(text, scale);

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

      DrawText(text, pos, scale, color);
   }

   float Renderer::GetCharHeight(unicode_t codepoint, float scale) const
   {
      return m_FontRenderer.GetHeight(codepoint, scale);
   }

   float Renderer::GetCharAdvance(unicode_t codepoint, float scale) const
   {
      return m_FontRenderer.GetAdvance(codepoint, scale);
   }

   math::vec2f Renderer::CalcBBox(const Text& text, float scale) const
   {
      math::vec2f res{0, 0};
      Text::Iterator iter(text);
      while(!iter.HasEnded())
      {
         res.x += GetCharAdvance(iter.Get(), scale);
         float h = GetCharHeight(iter.Get(), scale);
         res.y = res.y < h ? h : res.y;
         iter.Next();
      }
      return res;
   }


   math::vec2f Renderer::GetCharBaseScale() const
   {
      return m_FontRenderer.GetBaseScale();
   }

   void Renderer::SetCanvasScale(const math::vec2f& scale)
   {
      Shader* shader = GetShader(s_TextureShaderId);
      shader->Use();
      math::mat4f mat = math::getOrthoProj(scale.x, scale.y, -1.0f, 1.0f);
      shader->SetUniformMatrix4fv("vProjMat", mat);
      m_CanvasScale = scale;
      m_FontRenderer.SetCanvasScale(scale);
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
