#ifndef _TOTEM_UI_BUTTON_H_
#define _TOTEM_UI_BUTTON_H_

#include "Clickable.h"

namespace totem
{
   class ButtonHandler
   {
      public:
         virtual ~ButtonHandler() = default;

         virtual void OnDefault() {}
         virtual void OnHover() {}
         virtual void OnPush() {}
         virtual void OnClick() {}
   };

   class Button : public Clickable
   {
      public:
         Button(Ref<Font> font);
         virtual ~Button() = default;

         Button(const Button& other) = default;
         Button& operator=(const Button& other) = default;

         void SetColor(const math::vec4f& color);
         void SetTextColor(const math::vec4f& color);

         void SetText(const Text& text);

         // Caller owns handler, button only keeps
         // a pointer to it.
         void SetHandler(ButtonHandler* handler);

         virtual void OnUpdate(float /*deltaTime*/) override {}
         virtual void Draw(Renderer* renderer) const override;

      protected:
         virtual void OnDefault() override;
         virtual void OnHover() override;
         virtual void OnPush() override;
         virtual void OnClick() override;
      
      private:
         math::vec4f m_Color;
         math::vec4f m_TextColor;
         Ref<Font> m_Font;
         Text m_Text;
         ButtonHandler* m_Handler;
   };
}

#endif
