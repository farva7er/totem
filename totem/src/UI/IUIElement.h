#ifndef _TOTEM_UI_ELEMENT_H_
#define _TOTEM_UI_ELEMENT_H_

#include "Events.h"
#include "Renderer/Renderer.h"


namespace totem
{
   class IUIElement : public IEventListener
   {
   public:
      virtual ~IUIElement() = default;

      virtual unsigned int GetID() const = 0;

      virtual IUIElement* GetParent() const = 0;
      virtual void SetParent(IUIElement* el) = 0;

      // A way to get notified by a child that it is going to be deleted
      virtual void Forget(unsigned int elID) = 0;

      virtual void OnUpdate(float /*deltaTime*/) {};
      virtual void Draw(Renderer* renderer) const = 0;

   private:
      static unsigned int s_ID_Counter;
   public:
      static unsigned int ObtainID();
   };
}

#endif
