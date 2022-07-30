#ifndef _TOTEM_UI_CLICKABLE_H_
#define _TOTEM_UI_CLICKABLE_H_

#include "Element.h"

namespace totem
{
   class Clickable : public Element
   {
      public:
         Clickable(Element* parent = nullptr);

         Clickable(const Clickable& other);

         virtual ~Clickable() = default;

         Clickable& operator=(const Clickable& other);

         bool IsActive() const;
         void SetActive(bool isActive);

         virtual void OnEvent(Event& e) override;

      protected:
         virtual void OnDefault() {}
         virtual void OnHover() {}
         virtual void OnPush() {}
         virtual void OnClick() {}

      private:
         void OnMouseMove(MouseMoveEvent& e);
         void OnMousePressed(MousePressedEvent& e);
         void OnMouseReleased(MouseReleasedEvent& e);

         bool IsHovered(const math::vec2f& mouseCanvasCoords) const;

      private:
         enum class State { Default, Hovered, Pushed };

         State m_State;
         bool m_IsActive;
   };
}

#endif
