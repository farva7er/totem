#include "Texture.h"
#include "Log.h"

#include <glad/glad.h>
#include <stb_image.h>

Texture::~Texture()
{
   glDeleteTextures(1, &m_RendererId);
}

void Texture::Load()
{
   stbi_set_flip_vertically_on_load(true);
   unsigned char* data = 
      stbi_load(m_Path, &m_Width, &m_Height, &m_ChannelCount, 0);
   
   if(!data)
   {
      LOG_ERROR("Couldn't load texture: %s", m_Path);
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

   glGenTextures(1, &m_RendererId);
   glBindTexture(GL_TEXTURE_2D, m_RendererId);
   glTexImage2D(GL_TEXTURE_2D, 0, resourceFormat, 
         m_Width, m_Height, 0, openGLFormat, GL_UNSIGNED_BYTE, data);

   glTexParameteri(GL_TEXTURE_2D,   GL_TEXTURE_WRAP_S, GL_REPEAT);  
   glTexParameteri(GL_TEXTURE_2D,   GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D,   GL_TEXTURE_MIN_FILTER, 
                                    GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,   GL_TEXTURE_MAG_FILTER, 
                                    GL_LINEAR);
   stbi_image_free(data);

}

void Texture::Bind() const
{
   glBindTexture(GL_TEXTURE_2D, m_RendererId);  
}
