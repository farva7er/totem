#include "MovableElement.h"

namespace totem
{ 
   void MovableElement::SetPos(const math::vec2f& pos)
   {
      m_Pos = pos;
   }

   void MovableElement::SetScale(const math::vec2f& scale)
   {
      m_Scale = scale;
   }
}
