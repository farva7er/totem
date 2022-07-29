#include "Texture.h"
#include "Log.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace totem
{
   Texture::Texture(const unsigned char* data,
                     int width, int height, int channelCount)
      : Resource(nullptr, nullptr), m_Width(width), m_Height(height),
         m_ChannelCount(channelCount)
   {
      
      GLenum resourceFormat = 0, openGLFormat = 0;
      if (m_ChannelCount == 4)
      {
         resourceFormat = GL_RGBA8;
         openGLFormat = GL_RGBA;
         glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
      }
      else if (m_ChannelCount == 3)
      {
         resourceFormat = GL_RGB8;
         openGLFormat = GL_RGB;
         glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      }
      else if (m_ChannelCount == 1)
      {
         resourceFormat = GL_RED;
         openGLFormat = GL_RED;
         glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      }

      glGenTextures(1, &m_RenderAPIID);
      glBindTexture(GL_TEXTURE_2D, m_RenderAPIID);
      glTexImage2D(GL_TEXTURE_2D, 0, resourceFormat, 
            m_Width, m_Height, 0, openGLFormat, GL_UNSIGNED_BYTE, data);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      glTexParameteri(GL_TEXTURE_2D,   GL_TEXTURE_MIN_FILTER, 
            GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,   GL_TEXTURE_MAG_FILTER, 
            GL_LINEAR);

      SetLoaded();
   }

   Texture::~Texture()
   {
      glDeleteTextures(1, &m_RenderAPIID);
   }

   void Texture::Load()
   {
      stbi_set_flip_vertically_on_load(true);

      // m_ResourceId contains path to the image
      unsigned char* data = 
         stbi_load(GetName(), &m_Width, &m_Height, &m_ChannelCount, 0);

      if(!data)
      {
         LOG_ERROR("Couldn't load texture: %s", GetName());
         return;
      }

      GLenum resourceFormat = 0, openGLFormat = 0;
      if (m_ChannelCount == 4)
      {
         resourceFormat = GL_RGBA8;
         openGLFormat = GL_RGBA;

      }
      else if (m_ChannelCount == 3)
      {
         resourceFormat = GL_RGB8;
         openGLFormat = GL_RGB;
      }
      else if (m_ChannelCount == 1)
      {
         resourceFormat = GL_RED;
         openGLFormat = GL_RED;
      }

      glGenTextures(1, &m_RenderAPIID);
      glBindTexture(GL_TEXTURE_2D, m_RenderAPIID);
      glTexImage2D(GL_TEXTURE_2D, 0, resourceFormat, 
            m_Width, m_Height, 0, openGLFormat, GL_UNSIGNED_BYTE, data);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);  
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
      float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
      glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

      glTexParameteri(GL_TEXTURE_2D,   GL_TEXTURE_MIN_FILTER, 
            GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D,   GL_TEXTURE_MAG_FILTER, 
            GL_LINEAR);

      stbi_image_free(data);
      SetLoaded();
   }

   void Texture::Bind() const
   {
      glBindTexture(GL_TEXTURE_2D, m_RenderAPIID);  
   }
}
