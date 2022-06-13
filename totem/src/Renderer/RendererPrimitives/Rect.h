#ifndef _TOTEM_RENDERER_PRIMITIVES_RECT_H_
#define _TOTEM_RENDERER_PRIMITIVES_RECT_H_

#include "Math/Vec.h"
#include "Renderer/Texture.h"

namespace totem
{
   class Rect
   {
   public:
      
      math::vec2f GetPos() const;
      math::vec2f GetScale() const;
      math::vec4f GetColor() const;
      const char* GetImagePath() const;
      const Texture* GetTexture() const;
      const char* GetShaderId() const;
      float GetRotationAngle() const;
      math::vec2f GetRotationAxis() const;

      Rect& SetPos(const math::vec2f& pos);
      Rect& SetScale(const math::vec2f& scale);
      Rect& SetColor(const math::vec4f& color);
      Rect& SetImagePath(const char* imagePath);
      Rect& SetTexture(const Texture* texture);
      Rect& SetShaderId(const char* shaderId);
      Rect& SetRotationAngle(float degAngle);
      Rect& SetRotationAxis(const math::vec2f& axis);


      Rect();
      Rect(const Rect& other);
      Rect& operator= (const Rect& other);
      ~Rect();
   
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
