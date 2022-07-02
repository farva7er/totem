#include "Rendering/Primitives/Rect.h"

namespace totem
{ 
   Rect::Rect()
      :  m_Pos(0, 0), m_Scale(1, 1), m_Color(1, 1, 1, 1),
         m_RotationAngleDeg(0), m_RotationAxis(0, 0)
   {}

   math::vec2f Rect::GetPos() const
   {
      return m_Pos;
   }

   math::vec2f Rect::GetScale() const
   {
      return m_Scale;
   }

   math::vec4f Rect::GetColor() const
   {
      return m_Color;
   }

   float Rect::GetRotationAngle() const
   {
      return m_RotationAngleDeg;
   }

   math::vec2f Rect::GetRotationAxis() const
   {
      return m_RotationAxis;
   }

   Rect& Rect::SetPos(const math::vec2f& pos)
   {
      m_Pos = pos;
      return *this;
   }

   Rect& Rect::SetScale(const math::vec2f& scale)
   {
      m_Scale = scale;
      return *this;
   }

   Rect& Rect::SetColor(const math::vec4f& color)
   {
      m_Color = color;
      return *this;
   }

   Rect& Rect::SetRotationAngle(float degAngle)
   {
      m_RotationAngleDeg = degAngle;
      return *this;
   }

   Rect& Rect::SetRotationAxis(const math::vec2f& axis)
   {
      m_RotationAxis = axis;
      return *this;
   }
}
