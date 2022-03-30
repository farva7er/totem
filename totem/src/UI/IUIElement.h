#ifndef _TOTEM_UI_ELEMENT_H_
#define _TOTEM_UI_ELEMENT_H_

#include "Events.h"
#include "Renderer/Renderer.h"


namespace totem
{

   class IUIElementListener
   {
   public:
      virtual ~IUIElementListener() {}

      virtual void OnClick(int btn) {}
      virtual void OnLostHover() {}
      virtual void OnHover() {}
      virtual void OnPush() {}
   };

   class IUIElement : public IEventListener
   {
   public:
      virtual ~IUIElement() {};

      virtual const math::vec2f& GetPos() const = 0;
      virtual const math::vec2f& GetScale() const = 0;

      virtual void SetPos(const math::vec2f& pos) = 0;
      virtual void SetScale(const math::vec2f& scale) = 0;

      virtual void AddListener(IUIElementListener* listener) = 0;

      virtual void OnUpdate(float deltaTime) {};
      virtual void Draw(Renderer* renderer) const = 0;
   };

}

#endif
