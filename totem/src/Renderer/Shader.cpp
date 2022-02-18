#include <glad/glad.h>
#include <fstream>

#include "Shader.h"
namespace totem
{
   Shader::Shader(const char* resId,
                  const char* vShaderPath, const char* fShaderPath)
      : Resource(resId)
   {
      m_VShaderPath = new char[strlen(vShaderPath) + 1];
      strcpy(m_VShaderPath, vShaderPath);
      m_FShaderPath = new char[strlen(fShaderPath) + 1];
      strcpy(m_FShaderPath, fShaderPath);
   }

   Shader::~Shader() {
      delete [] m_VShaderPath;
      delete [] m_FShaderPath;
   }

   void Shader::Load()
   {
      char* vShaderSrc;
      char* fShaderSrc;

      vShaderSrc = GetShaderSrc(m_VShaderPath);
      fShaderSrc = GetShaderSrc(m_FShaderPath);
      Compile(vShaderSrc, fShaderSrc);
      delete [] vShaderSrc;
      delete [] fShaderSrc;
      m_IsLoaded = true;
   }

   void Shader::Use()
   {
      glUseProgram(m_RenderAPIId);
   }

   char* Shader::GetShaderSrc(const char* filePath) const
   {
      std::ifstream is(filePath, std::ifstream::binary);
      if(is)
      {
         is.seekg(0, is.end);
         int length = is.tellg();
         is.seekg(0, is.beg);

         char* buffer = new char[length + 1];

         is.read(buffer, length);
         is.close();
         buffer[length] = 0;
         //LOG_INFO(buffer);
         return buffer;
      }
      else
      {
         LOG_FATAL("Shader Src file cannot be opened %s", filePath);
         return nullptr;
      }
   }

   void Shader::Compile(const char* vShaderSrc, const char* fShaderSrc)
   {
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

      m_RenderAPIId = glCreateProgram();
      glAttachShader(m_RenderAPIId, vShader);
      glAttachShader(m_RenderAPIId, fShader);
      glLinkProgram(m_RenderAPIId);

      glGetProgramiv(m_RenderAPIId, GL_LINK_STATUS, &success);
      if(!success)
      {
         glGetProgramInfoLog(m_RenderAPIId, 512, NULL, infoLog);
         LOG_FATAL("Shader Program linking failed: %s", infoLog);
         return;
      }

      glDeleteShader(vShader);
      glDeleteShader(fShader);
   }

   void Shader::SetUniformMatrix4fv(const char* name, const math::mat4f& val)
   {
      int loc = glGetUniformLocation(m_RenderAPIId, name);
      glUniformMatrix4fv(loc, 1, GL_TRUE, val.ToArray());
   }

   void Shader::SetUniform4f(const char* name, const math::vec4f& val)
   {
      int loc = glGetUniformLocation(m_RenderAPIId, name);
      glUniform4f(loc, val.x, val.y, val.z, val.w);
   }
}
