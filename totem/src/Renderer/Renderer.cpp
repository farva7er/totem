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

   Renderer::Renderer(Window *window)
      : m_SceneSize(10.0f)
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

      glGenTextures(1, &m_WhiteTexture);
      glBindTexture(GL_TEXTURE_2D, m_WhiteTexture);

      uint32_t whiteTexData = 0xffffffff;
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 
         1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &whiteTexData);

      m_TextureShaderId = "textureShader";
      ResourceManager& rm = ResourceManager::GetInstance();
      rm.AddResource(new Shader(
               m_TextureShaderId,
               "resources/shaders/Vtexture.glsl",
               "resources/shaders/Ftexture.glsl"));

      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   }

   Renderer::~Renderer()
   {
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

   void Renderer::DrawRect(math::vec2f pos, math::vec2f scale,
                           math::vec4f color)
   {
      Shader* shader = GetShader(m_TextureShaderId);
      shader->Use();
      math::mat4f mat = math::getTranslate(pos.x * 10.0 * m_AspectRatio, 
                                          pos.y * 10.0f)
                     *  math::getScale(scale.x, scale.y);
      shader->SetUniformMatrix4fv("vModelMat", mat);
      shader->SetUniform4f("fColor", color);
      glBindTexture(GL_TEXTURE_2D, m_WhiteTexture);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   }

   void Renderer::DrawRect(math::vec2f pos, math::vec2f scale,
                           const char *imagePath, 
                           math::vec4f tintColor)
   {
      Shader* shader = GetShader(m_TextureShaderId);
      shader->Use();
      math::mat4f mat = math::getTranslate(pos.x * 10.0 * m_AspectRatio, 
                                          pos.y * 10.0f)
                     *  math::getScale(scale.x, scale.y);
      shader->SetUniformMatrix4fv("vModelMat", mat);
      shader->SetUniform4f("fColor", tintColor);

      ResourceManager& rm = ResourceManager::GetInstance();
      Texture *texture = rm.GetResource<Texture>(imagePath);
      if(!texture)
         texture = rm.LoadResource(new Texture(imagePath));
      texture->Bind();
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   }


   void Renderer::DrawImage(const char* imagePath, math::vec2f pos,
                           float scale, math::vec4f tintColor)
   {
      ResourceManager& rm = ResourceManager::GetInstance();
      Texture *texture = rm.GetResource<Texture>(imagePath);
      if(!texture)
         texture = rm.LoadResource(new Texture(imagePath));
      float aspectRatio = texture->GetWidth()/(float)texture->GetHeight();
      DrawRect(pos, math::vec2f(scale * aspectRatio, scale), imagePath,
               tintColor);
   }


   void Renderer::DrawBackground(const char* imagePath)
   {
      DrawRect(math::vec2f(0, 0),
               math::vec2f(m_SceneSize * m_AspectRatio, m_SceneSize),
               imagePath);
   }

   void Renderer::SetAspectRatio(float aspectRatio)
   {
      Shader* shader = GetShader(m_TextureShaderId);
      shader->Use();
      math::mat4f mat = math::getOrthoProj(m_SceneSize * aspectRatio, 
                                           m_SceneSize, -1.0f, 1.0f);
      shader->SetUniformMatrix4fv("vProjMat", mat);
      //LOG_INFO("aspectRatio: %f", aspectRatio);
      m_AspectRatio = aspectRatio;
   } void Renderer::HandleResize(unsigned int width, unsigned int height)
   {
      glViewport(0, 0, width, height);
      SetAspectRatio(width/(float)height);
      //LOG_INFO("Framebuffer Resized - W: %u H: %u", width, height);
   }
}
