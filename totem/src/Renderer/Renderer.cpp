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
         : m_SceneSize(10.0f), m_FontRenderer(this)
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
         m_Window->AddEventListener(this);
         
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
         m_Window->RemoveEventListener(this);
      }

      void Renderer::OnEvent(Event& e)
      {
         if(e.GetType() == EventType::FramebufferResize)
         {
            FramebufferResizeEvent& fre = e.Cast<FramebufferResizeEvent>();
            HandleResize(fre.GetWidth(), fre.GetHeight());
         }
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
            mat = math::getTranslate(pos.x * 10.0 * m_AspectRatio, 
                                                pos.y * 10.0f)
                        *  math::getScale(scale.x, scale.y);
         }
         else
         {
            // Draw rotated rect
            mat = 
               math::getTranslate(pos.x * 10.0 * m_AspectRatio, 
                                             pos.y * 10.0f)
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
                        .SetScale(math::vec2f(scale*aspectRatio, scale))
                        .SetImagePath(imagePath)
                        .SetColor(tintColor)
                        .Construct();
      DrawRect(rect);
   }


   void Renderer::DrawBackground(const char* imagePath)
   {
      math::vec2f scale(m_SceneSize * m_AspectRatio, m_SceneSize);
      Rect rect = Rect::Builder()
                  .SetScale(scale)
                  .SetImagePath(imagePath)
                  .Construct();
      DrawRect(rect);
   }

   void Renderer::DrawText(const char* str, const math::vec2f& pos,
                           float scale, const math::vec4f& color)
   {
      const float scaleFactor = 1.0f;
      math::vec2f currPos = pos;
      for(; *str; str++)
      {
         m_FontRenderer.DrawChar(*str, currPos, scale * scaleFactor,
                                 color);
         currPos.x += m_FontRenderer.GetAdvanceNormal(*str, scale);
      }
   }

   void Renderer::DrawControlledText(const char* str, 
                              const math::vec2f& boxPos,
                              const math::vec2f& boxScale,
                              float scale,
                              const math::vec4f& color,
                              int alignFlags)
   {
      math::vec2f pos;
      math::vec2f textSize = CalcTextSize(str, scale);

      math::vec2f leftTop(
         boxPos.x - CamUnitXToNormal(boxScale.x),
         boxPos.y + CamUnitYToNormal(boxScale.y)
      );

      math::vec2f rightBottom(
         boxPos.x + CamUnitXToNormal(boxScale.x),
         boxPos.y - CamUnitYToNormal(boxScale.y)
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

      DrawText(str, pos, scale, color);
   }

   math::vec2f Renderer::CalcTextSize(const char* str, float scale) const
   {
      math::vec2f size;

      for(; *str; str++)
      {
         size.x += m_FontRenderer.GetAdvanceNormal(*str, scale);
         float height = m_FontRenderer.GetHeightNormal(*str, scale);
         if(height > size.y)
            size.y = height;
      }
      return size;
   }

   void Renderer::SetAspectRatio(float aspectRatio)
   {
      Shader* shader = GetShader(s_TextureShaderId);
      shader->Use();
      math::mat4f mat = math::getOrthoProj(m_SceneSize * aspectRatio, 
                                           m_SceneSize, -1.0f, 1.0f);
      shader->SetUniformMatrix4fv("vProjMat", mat);
      //LOG_INFO("aspectRatio: %f", aspectRatio);
      m_AspectRatio = aspectRatio;

      m_FontRenderer.SetAspectRatio(aspectRatio);
   }

   void Renderer::HandleResize(unsigned int width, unsigned int height)
   {
      glViewport(0, 0, width, height);
      SetAspectRatio(width/(float)height);
      //LOG_INFO("Framebuffer Resized - W: %u H: %u", width, height);
   }

   float Renderer::PixelUnitXToCam(int px) const
   {
      float res = (px / (float)m_Window->GetFBWidth()) * m_SceneSize *
                                                      m_AspectRatio;
      return res;
   }

   float Renderer::PixelUnitYToCam(int py) const
   {
      float res = (py / (float)m_Window->GetFBHeight()) * m_SceneSize;
      return res;
   }

   float Renderer::PixelUnitXToNormal(int px) const
   {
      float res = (px / (float)m_Window->GetFBWidth());
      return res;
   }

   float Renderer::PixelUnitYToNormal(int py) const
   {
      float res = (py / (float)m_Window->GetFBHeight());
      return res;
   }

   math::vec2f Renderer::ScreenToNormal(math::vec2i scrCoords) const
   {
      float x = scrCoords.x / (float)m_Window->GetWidth();
      x = 2*x - 1; // map from (0; 1) to (-1; 1)
   
      float y = scrCoords.y / (float)m_Window->GetHeight();
      y = -(2*y - 1); // map from (0; 1) to (-1; 1)
                   
      return math::vec2f(x, y);
   }

   float Renderer::CamUnitXToNormal(float camX) const
   {
      float res = camX / (m_SceneSize * m_AspectRatio) ;
      return res;
   }

   float Renderer::CamUnitYToNormal(float camY) const
   {
      float res = camY / m_SceneSize;
      return res;
   }

   float Renderer::GetSceneSize() const
   {
      return m_SceneSize;
   }

   math::vec2f Renderer::GetContentScale() const
   {
      return math::vec2f(m_Window->GetContentScaleX(),
                        m_Window->GetContentScaleY());
   }
}
