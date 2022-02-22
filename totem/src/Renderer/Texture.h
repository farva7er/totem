#ifndef _TOTEM_TEXTURE_H_
#define _TOTEM_TEXTURE_H_

#include "ResourceManager.h"

namespace totem
{
   class Texture : public Resource
   {
      public:
         // set m_ResourceId to imagePath
         Texture(const char* imagePath)
            : Resource(imagePath) {}

         Texture(const unsigned char* data,
                     int width, int height, int channelCount);
 
         virtual ~Texture() override;

         virtual ResourceType GetType() const override 
         { return ResourceType::Image; }

         static ResourceType GetStaticType()
         { return ResourceType::Image; }
         void Bind() const;

         int GetWidth() const { return m_Width; }
         int GetHeight() const { return m_Height; }
         int GetChannelCount() const { return m_ChannelCount; }

      private:
         virtual void Load() override;

      private:
         unsigned int m_RenderAPIId;
         int m_Width, m_Height;
         int m_ChannelCount;
   };
}
#endif
