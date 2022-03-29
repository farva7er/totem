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
      class SceneResizeEvent : public Event
      {
      public:
         SceneResizeEvent(float xScale, float yScale)
            : m_XScale(xScale), m_YScale(yScale) {}

         virtual std::string ToString() const override
         {
            std::stringstream ss;
            ss << "Scene Resized Event: (" << 
               m_XScale << ", " << m_YScale <<")";
            return ss.str();
         }

      private:
         float m_XScale, m_YScale;
      };

   public:
      UIManager(Renderer* renderer);
      ~UIManager();

      IButton* CreateButton(ButtonType type);

      void OnUpdate(float deltaTime);
      virtual void OnEvent(Event& e) override;

      Renderer* GetRenderer() const;

   private:
      void AddElement(IUIElement* element);
      void DispatchEvent(Event& e) const;
   private:
      struct UIElementNode
      {
         IUIElement* element;
         UIElementNode* next;
         UIElementNode(IUIElement* el, UIElementNode* next = nullptr)
            : element(el), next(next) {}
         ~UIElementNode();
      };

      UIElementNode* m_Elements;
      Renderer* m_Renderer;
   };
}

#endif
