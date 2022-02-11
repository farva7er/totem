#include <glad/glad.h>

#include "Renderer.h"
#include "Assert.h"

namespace totem
{
   bool Renderer::s_OpenGLInitialized = false;

   Renderer::Renderer(Window *window)
   {
      m_Window = window;

      // There should be a valid context for glad to initialize 
      m_Window->MakeCurrent();
      if(!s_OpenGLInitialized)
      { 
         if (!gladLoadGLLoader((GLADloadproc) m_Window->GetOpenGLLoaderFunc()))
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
         -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
          0.5f, -0.5f, 0.0f,     1.0f, 0.0f,
          0.5f,  0.5f, 0.0f,     1.0f, 1.0f,
         -0.5f,  0.5f, 0.0f,     0.0f, 1.0f
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



      const char* vShaderSrc = 
         "#version 330 core\n"
         "layout (location = 0) in vec3 vPos; "
         "layout (location = 1) in vec2 vTexCoords; "
         "out vec2 TexCoords; "
         "uniform mat4 vModelMat; "
         "uniform mat4 vProjMat; "
         "void main() "
         "{ "
         "  gl_Position = vProjMat * vModelMat * vec4(vPos, 1.0f); "
         "  TexCoords = vTexCoords; "
         "} "
      ;

      const char* fShaderSrc =
         "#version 330 core\n"
         "in vec2 TexCoords; "
         "out vec4 FragColor; "
         "uniform vec4 fColor;"
         "uniform sampler2D texSampler; "
         "void main() "
         "{ "
         "  FragColor = fColor * texture(texSampler, TexCoords); " 
         "} "
      ;

      unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
      unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
      int success;
      char infoLog[512];
      glShaderSource(vShader, 1, &vShaderSrc, nullptr);
      glShaderSource(fShader, 1, &fShaderSrc, nullptr);
      glCompileShader(vShader);
      glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
      if(!success)
      {
         glGetShaderInfoLog(vShader, 512, nullptr, infoLog);
         LOG_FATAL("Vertex Shader compilation failed: %s", infoLog);
         return;
      }
      glCompileShader(fShader);
      glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
      if(!success)
      {
         glGetShaderInfoLog(fShader, 512, nullptr, infoLog);
         LOG_FATAL("Fragment Shader compilation failed: %s", infoLog);
         return;
      }

      m_ShaderProgram = glCreateProgram();
      glAttachShader(m_ShaderProgram, vShader);
      glAttachShader(m_ShaderProgram, fShader);
      glLinkProgram(m_ShaderProgram);

      glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
      if(!success)
      {
         glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
         LOG_FATAL("Shader Program linking failed: %s", infoLog);
         return;
      }

      glUseProgram(m_ShaderProgram);
      glDeleteShader(vShader);
      glDeleteShader(fShader);

      HandleResize(m_Window->GetFBWidth(), m_Window->GetFBHeight());
   }

   Renderer::~Renderer()
   {
      m_Window->RemoveEventListener(this);
   }

   void Renderer::OnEvent(Event& e)
   {
      if(e.GetType() == EventType::FramebufferResize)
      {
         FramebufferResizeEvent& fre = 
            CAST_EVENT(FramebufferResizeEvent, e);
         HandleResize(fre.GetWidth(), fre.GetHeight());
      }
   }
 
   void Renderer::Clear(float r, float g, float b, float a)
   {
      glClearColor(r, g, b, a);
      glClear(GL_COLOR_BUFFER_BIT);
   }

   void Renderer::DrawRect(math::vec2f pos, math::vec2f scale,
                           math::vec4f color)
   { 
      int modelMatLoc = glGetUniformLocation(m_ShaderProgram, "vModelMat");
      math::mat4f mat = math::getTranslate(pos.x * 10.0 * m_AspectRatio, 
                                          pos.y * 10.0f)
                     *  math::getScale(scale.x, scale.y);
      glUniformMatrix4fv(modelMatLoc, 1, GL_TRUE, mat.ToArray());
      int colorLoc = glGetUniformLocation(m_ShaderProgram, "fColor");
      glUniform4f(colorLoc, color.x, color.y, color.z, color.w);
      glBindTexture(GL_TEXTURE_2D, m_WhiteTexture);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   }

   void Renderer::DrawRect(math::vec2f pos, math::vec2f scale,
                           const char *imagePath, 
                           math::vec4f tintColor)
   {
      int modelMatLoc = glGetUniformLocation(m_ShaderProgram, "vModelMat");
      math::mat4f mat = math::getTranslate(pos.x * 10.0 * m_AspectRatio, 
                                          pos.y * 10.0f)
                     *  math::getScale(scale.x, scale.y);
      glUniformMatrix4fv(modelMatLoc, 1, GL_TRUE, mat.ToArray());
      int colorLoc = glGetUniformLocation(m_ShaderProgram, "fColor");
      glUniform4f(colorLoc, 
            tintColor.x, tintColor.y, tintColor.z, tintColor.w);

      ResourceManager& rm = ResourceManager::GetInstance();
      Texture *texture = rm.GetResource<Texture>(imagePath);
      if(!texture)
         texture = rm.LoadResource(new Texture(imagePath));
      texture->Bind();
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   }

   void Renderer::SetAspectRatio(float aspectRatio)
   {
      int projMatLoc = glGetUniformLocation(m_ShaderProgram, "vProjMat");
      math::mat4f mat = math::getOrthoProj(10.0f * aspectRatio, 10.0f,
                                       -1.0f, 1.0f);
      //LOG_INFO("aspectRatio: %f", aspectRatio);
      glUniformMatrix4fv(projMatLoc, 1, GL_TRUE, mat.ToArray());
      m_AspectRatio = aspectRatio;
   }


   void Renderer::HandleResize(unsigned int width, unsigned int height)
   {
      glViewport(0, 0, width, height);
      SetAspectRatio(width/(float)height);
      //LOG_INFO("Framebuffer Resized - W: %u H: %u", width, height);
   }
}
