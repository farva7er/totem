#ifndef _TOTEM_UI_ELEMENT_H_
#define _TOTEM_UI_ELEMENT_H_

#include "Events.h"
#include "Renderer/Renderer.h"


namespace totem
{
   class UIManager;

   class UIElement : public IEventListener
   {
      friend class UIManager;
   protected:
      UIElement(UIManager* manager);
      virtual ~UIElement();

      virtual void OnUpdate(float deltaTime) = 0;
      virtual void Draw() = 0;

      Renderer* GetRenderer() const;

   private:
      UIManager* m_Master;
   };
}

#endif
