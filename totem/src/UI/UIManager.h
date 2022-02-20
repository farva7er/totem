#ifndef _TOTEM_UIMANAGER_H_
#define _TOTEM_UIMANAGER_H_

#include "Events.h"
#include "Renderer/Renderer.h"
#include "Button.h"

namespace totem
{

   // UIManager
   // Basic Usage:
   // Create a UIManager
   // for example store it as a field in a class like that
   // class A
   // {
   //    UIManager m_UIManager;
   // }
   //
   // Create a UIElement using one of the factory methods
   // in UIManager.
   // See Button.h for example on how to create a button
   
     
   class UIElement;

   class UIManager : public IEventListener
   {
   public:
      UIManager(Renderer* renderer);
      ~UIManager();

      Button* CreateButton(ButtonType type, IClickListener* listener);

      void OnUpdate(float deltaTime);
      void Forget(UIElement* element);
      virtual void OnEvent(Event& e) override;

      Renderer* GetRenderer() const;

   private:
      void AddElement(UIElement* element);

   private:
      struct UIElementNode
      {
         UIElement* element;
         UIElementNode* next;
         UIElementNode(UIElement* el, UIElementNode* next = nullptr)
            : element(el), next(next) {}
         ~UIElementNode();
      };

      UIElementNode* m_Elements;
      Renderer* m_Renderer;
   };
}

#endif
