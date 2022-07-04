#ifndef _TOTEM_TEXTURE_H_
#define _TOTEM_TEXTURE_H_

#include "Core/ResourceManager.h"

namespace totem
{
   class Texture : public Resource
   {
      public:
         // Constructor for ResourceManager.
         Texture(const char* name, ResourceManager* manager)
            : Resource(name, manager) {}

         // Non-managed resource.
         // Use this constructor to create a texture
         // with in-memory data.
         Texture(const unsigned char* data,
                     int width, int height, int channelCount);
 
         virtual ~Texture() override;

         virtual void Load() override;

         void Bind() const;

         int GetWidth() const { return m_Width; }
         int GetHeight() const { return m_Height; }
         int GetChannelCount() const { return m_ChannelCount; }

      private:
         unsigned int m_RenderAPIID;
         int m_Width, m_Height;
         int m_ChannelCount;
   };
}
#endif
