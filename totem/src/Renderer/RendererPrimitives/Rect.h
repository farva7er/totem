#ifndef _TOTEM_RENDERER_PRIMITIVES_RECT_H_
#define _TOTEM_RENDERER_PRIMITIVES_RECT_H_

#include "Math/Vec.h"
#include "Renderer/Texture.h"

namespace totem
{
   class Rect
   {
   public:
      class Builder
      {
      public:
         Builder();
         ~Builder();
         Builder& SetPos(const math::vec2f& pos);
         Builder& SetScale(const math::vec2f& scale);
         Builder& SetColor(const math::vec4f& color);
         Builder& SetImagePath(const char* imagePath);
         Builder& SetTexture(const Texture* texture);
         Builder& SetShaderId(const char* shaderId);
         Builder& SetRotationAngle(float degAngle);
         Builder& SetRotationAxis(const math::vec2f& axis);

         void Reset();
         Rect Construct();
      private:
         Rect* m_Result;
      };

      math::vec2f GetPos() const;
      math::vec2f GetScale() const;
      math::vec4f GetColor() const;
      const char* GetImagePath() const;
      const Texture* GetTexture() const;
      const char* GetShaderId() const;
      float GetRotationAngle() const;
      math::vec2f GetRotationAxis() const;

      Rect(const Rect& other);
      Rect& operator= (const Rect& other);
      ~Rect();
   private:
      Rect();
   
   private:
      math::vec2f m_Pos;
      math::vec2f m_Scale;
      math::vec4f m_Color;
      char* m_ImagePath;
      const Texture* m_Texture;
      char* m_ShaderId;
      float m_RotationAngleDeg;
      math::vec2f m_RotationAxis;
   };
};

#endif
