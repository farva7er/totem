#include "CheckButton.h"

namespace totem
{
   CheckButton::CheckButton()
      :  m_IsChecked(false),
         m_Color({ 0.2f, 0.2f, 0.2f, 1.0f })
   {}

   CheckButton::CheckButton(const CheckButton& other)
      : InteractiveElementImpl(other)
   {
      m_IsChecked = false;
      m_Color = other.m_Color;
   }

   CheckButton& CheckButton::operator=(const CheckButton& other)
   {
      InteractiveElementImpl::operator=(other);
      m_IsChecked = false;
      m_Color = other.m_Color;
      return *this;
   }

   void CheckButton::Draw(Renderer* renderer) const
   {

      Rect outerRect = Rect::Builder()
                        .SetPos(m_Pos)
                        .SetScale(m_Scale)
                        .SetColor(m_Color)
                        .Construct();

      renderer->DrawRect(outerRect);

      if(m_IsChecked)
      {
         Rect innerRect = Rect::Builder()
                           .SetPos(m_Pos)
                           .SetScale(m_Scale * 0.7f)
                           .SetColor({0, 0, 0, 0.7f})
                           .Construct();

         renderer->DrawRect(innerRect); 
      } 
   }


   void CheckButton::SetScale(const math::vec2f& scale)
   {
      float minScale = scale.x < scale.y ? scale.x : scale.y;

      InteractiveElementImpl::SetScale({ minScale, minScale });
   }


   void CheckButton::OnClick(int /*button*/)
   {
      m_IsChecked = m_IsChecked ? false : true;
   }
}
