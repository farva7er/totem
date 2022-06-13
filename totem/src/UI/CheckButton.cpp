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
      const math::vec4f grayColor{0.5f, 0.5f, 0.5f, 0.5f};
      const math::vec4f& color = IsActive() ? m_Color : grayColor;

      Rect outerRect;
      outerRect.SetPos(m_Pos)
               .SetScale(m_Scale)
               .SetColor(color);

      renderer->DrawRect(outerRect);

      if(m_IsChecked)
      {
         Rect innerRect;
         innerRect.SetPos(m_Pos)
                  .SetScale(m_Scale * 0.5f)
                  .SetColor({0, 0, 0, 0.7f});

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
      SetChecked(!IsChecked());
   }

   bool CheckButton::IsChecked() const
   {
      return m_IsChecked;
   }

   void CheckButton::SetChecked(bool checked)
   {
      m_IsChecked = checked;
   }
}
