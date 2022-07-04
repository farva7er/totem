#ifndef _TOTEM_SHADER_H_
#define _TOTEM_SHADER_H_

#include "Math/Mat.h"
#include "Math/Vec.h"
#include "Core/ResourceManager.h"

namespace totem
{
   class Shader : public Resource
   {
      public:
         Shader(const char* name, ResourceManager* manager);
         ~Shader();

         virtual void Load() override;
         void Use();

         void SetUniformMatrix4fv(const char* name, const math::mat4f& val);
         void SetUniform4f(const char* name, const math::vec4f& val);
      private:
         bool Compile(const char* vShaderSrc, const char* fShaderSrc);
         char* GetShaderFullSrc(const char* name) const;
         char* ExtractSection(const char* buff, const char* sectionName);
         static int GetLineLength(const char* line);

      private:
         unsigned int m_RenderAPIID;
   };
}

#endif
