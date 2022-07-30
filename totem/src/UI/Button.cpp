#include "Button.h"

namespace totem
{

   Button::Button(Ref<Font> font)
      : m_Color(0.5f, 0.5f, 0.5f, 0.5f),
      m_TextColor(1, 1, 1, 1), m_Font(font), m_Handler(nullptr)
   {}

   void Button::SetColor(const math::vec4f& color)
   {
      m_Color = color;
   }

   void Button::SetTextColor(const math::vec4f& color)
   {
      m_TextColor = color;
   }

   void Button::SetText(const Text& text)
   {
      m_Text = text;
   }

   void Button::SetHandler(ButtonHandler* handler)
   {
      m_Handler = handler;
   }

   void Button::Draw(Renderer* renderer) const
   {
      if(!IsActive())
         return;

      Rect rect;
      rect
         .SetPos(GetPos())
         .SetScale(GetScale())
         .SetColor(m_Color);

      renderer->DrawRect(rect);

      if(m_Text.GetLength() <= 0)
         return;

      renderer->DrawAlignedText(m_Text, GetPos(), GetScale(),
                              *m_Font, GetScale().y / 1.5f, m_TextColor,
                              TextAlign::VCenter | TextAlign::HCenter);
   }

   void Button::OnDefault()
   {
      if(m_Handler)
         m_Handler->OnDefault();
   }

   void Button::OnHover()
   {
      if(m_Handler)
         m_Handler->OnHover();
   }

   void Button::OnPush()
   {
      if(m_Handler)
         m_Handler->OnPush();
   }

   void Button::OnClick()
   {
      if(m_Handler)
         m_Handler->OnClick();
   }
}
