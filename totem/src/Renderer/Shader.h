#ifndef _TOTEM_SHADER_H_
#define _TOTEM_SHADER_H_

#include "ResourceManager.h"
#include "Math/Mat.h"
#include "Math/Vec.h"

namespace totem
{
   class Shader : public Resource
   {
   public:
      Shader(const char* resId, 
            const char* vShaderPath, const char* fShaderPath);
      ~Shader();

      virtual void Load() override;
      void Use();

      virtual ResourceType GetType() const override 
      { return ResourceType::Shader; }

      static ResourceType GetStaticType()
      { return ResourceType::Shader; }

      void SetUniformMatrix4fv(const char* name, const math::mat4f& val);
      void SetUniform4f(const char* name, const math::vec4f& val);
   private:
      void Compile(const char* vShaderSrc, const char* fShaderSrc);
      char* GetShaderSrc(const char* filePath) const;

   private:
      unsigned int m_RenderAPIId;
      char* m_VShaderPath;
      char* m_FShaderPath;
   };
}

#endif
