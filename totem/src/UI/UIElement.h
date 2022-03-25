#ifndef _TOTEM_UI_ELEMENT_H_
#define _TOTEM_UI_ELEMENT_H_

#include "Events.h"
#include "Renderer/Renderer.h"


namespace totem
{
   class UIElement : public IEventListener
   {
   public:
      virtual ~UIElement() {};

      virtual void OnUpdate(float deltaTime) {};
      virtual void Draw(Renderer* renderer) const = 0;
   };
}

#endif
