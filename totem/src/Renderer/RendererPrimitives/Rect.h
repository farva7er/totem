#ifndef _TOTEM_RENDERER_PRIMITIVES_RECT_H_
#define _TOTEM_RENDERER_PRIMITIVES_RECT_H_

#include "Math/Vec.h"

namespace totem
{
   class Rect
   {
      public:
         
         math::vec2f GetPos() const;
         math::vec2f GetScale() const;
         math::vec4f GetColor() const;
         float GetRotationAngle() const;
         math::vec2f GetRotationAxis() const;

         Rect& SetPos(const math::vec2f& pos);
         Rect& SetScale(const math::vec2f& scale);
         Rect& SetColor(const math::vec4f& color);
         Rect& SetRotationAngle(float degAngle);
         Rect& SetRotationAxis(const math::vec2f& axis);


         Rect();
         ~Rect() = default;
         Rect(const Rect& other) = default;
         Rect& operator= (const Rect& other) = default;
      
      private:
         math::vec2f m_Pos;
         math::vec2f m_Scale;
         math::vec4f m_Color;
         float m_RotationAngleDeg;
         math::vec2f m_RotationAxis;
   };
};

#endif
