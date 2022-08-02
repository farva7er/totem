#include <glad/glad.h>
#include <fstream>

#include "Shader.h"

namespace totem
{
   Shader::Shader(const char* name, ResourceManager* manager)
      : Resource(name, manager), m_RenderAPIID(0)
   {}

   Shader::~Shader()
   {
      glDeleteProgram(m_RenderAPIID);
   }

   void Shader::Load()
   {
      char *shaderFullSrc = nullptr, *vShaderSrc = nullptr,
            *fShaderSrc = nullptr;
      bool res;

      if(IsLoaded())
         return;

      shaderFullSrc = GetShaderFullSrc(GetName());
      if(!shaderFullSrc)
         goto cleanup;

      vShaderSrc = ExtractSection(shaderFullSrc, "vertex shader");
      if(!vShaderSrc)
      {
         LOG_ERROR("Vertex shader not found: %s", GetName());
         goto cleanup;
      }
      fShaderSrc = ExtractSection(shaderFullSrc, "fragment shader");
      if(!fShaderSrc)
      {
         LOG_ERROR("Fragment shader not found: %s", GetName());
         goto cleanup;
      }

      res = Compile(vShaderSrc, fShaderSrc);
      if(!res)
         goto cleanup;

      SetLoaded();

   cleanup:
      if(vShaderSrc) delete [] vShaderSrc;
      if(fShaderSrc) delete [] fShaderSrc;
      if(shaderFullSrc) delete [] shaderFullSrc;
   }

   void Shader::Use()
   {
      glUseProgram(m_RenderAPIID);
   }

   char* Shader::ExtractSection(const char* buff, const char* sectionName)
   {
      // Every section should start with this marker
      const char sectionMarker[] = "##";
      int markerLen = strlen(sectionMarker);
      const char* sectionStart = nullptr;
      int sectionLen = 0;
      while(*buff)
      {
         if(0 == strncmp(buff, sectionMarker, markerLen))
         {
            // Marker found, so this is the beginning of a new section.
            if(0 == strncmp(buff + markerLen,
                              sectionName, strlen(sectionName)))
            {
               // Found beginning of the searched section.
               sectionStart = buff;
            }
            else if(sectionStart)
            {
               // Found end of the searched section by
               // finding a start of another section.
               sectionLen = buff - sectionStart; 
               break;
            }
         }

         // Go to next line.
         buff += GetLineLength(buff);
      }

      // Section not found.
      if(!sectionStart) return nullptr;

      // The searched section is the last one in buff.
      if(!sectionLen) sectionLen = buff - sectionStart;

      char* res = new char[sectionLen + 1];
      strncpy(res, sectionStart, sectionLen);
      res[sectionLen] = 0;
      return res;
   }

   int Shader::GetLineLength(const char* line)
   {
      int len = 0;
      while(*line)
      {
         len++;
         if(*line == '\n')
            break;
         line++;
      }
      return len;
   }

   char* Shader::GetShaderFullSrc(const char* name) const
   {
      std::ifstream is(name, std::ifstream::binary);
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
         LOG_FATAL("Shader Src file cannot be opened %s", name);
         return nullptr;
      }
   }

   

   bool Shader::Compile(const char* vShaderSrc, const char* fShaderSrc)
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
         return false;
      }
      glCompileShader(fShader);
      glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
      if(!success)
      {
         glGetShaderInfoLog(fShader, 512, nullptr, infoLog);
         LOG_FATAL("Fragment Shader compilation failed: %s", infoLog);
         return false;
      }

      m_RenderAPIID = glCreateProgram();
      glAttachShader(m_RenderAPIID, vShader);
      glAttachShader(m_RenderAPIID, fShader);
      glLinkProgram(m_RenderAPIID);

      glGetProgramiv(m_RenderAPIID, GL_LINK_STATUS, &success);
      if(!success)
      {
         glGetProgramInfoLog(m_RenderAPIID, 512, NULL, infoLog);
         LOG_FATAL("Shader Program linking failed: %s", infoLog);
         return false;
      }

      glDetachShader(m_RenderAPIID, vShader);
      glDetachShader(m_RenderAPIID, fShader);

      glDeleteShader(vShader);
      glDeleteShader(fShader);
      return true;
   }

   void Shader::SetUniformMatrix4fv(const char* name, const math::mat4f& val)
   {
      int loc = glGetUniformLocation(m_RenderAPIID, name);
      glUniformMatrix4fv(loc, 1, GL_TRUE, val.ToArray());
   }

   void Shader::SetUniform4f(const char* name, const math::vec4f& val)
   {
      int loc = glGetUniformLocation(m_RenderAPIID, name);
      glUniform4f(loc, val.x, val.y, val.z, val.w);
   }
}
