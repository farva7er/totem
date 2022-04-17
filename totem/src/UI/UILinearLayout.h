#ifndef _TOTEM_UIMANAGER_H_
#define _TOTEM_UIMANAGER_H_

#include "Events.h"
#include "Renderer/Renderer.h"
#include "Button.h"
#include "IUIElement.h"

namespace totem
{   
   class UILinearLayout : public MovableElement
   {
     
   public:
      UILinearLayout();
      virtual ~UILinearLayout();
      UILinearLayout(const UILinearLayout& other) = delete;
      UILinearLayout& operator=(const UILinearLayout& other) = delete;

      virtual void OnEvent(Event& e) override;
      virtual void OnUpdate(float deltaTime) override;
      virtual void Draw(Renderer* renderer) const override;

      virtual void SetPos(const math::vec2f& pos) override;
      virtual void SetScale(const math::vec2f& scale) override;
      void SetSpacing(float spacing);

      void AddElement(IMovableElement* element);
   private:
      struct ElementNode
      {
         IMovableElement* element;
         ElementNode* next;
         ElementNode(IMovableElement* el, ElementNode* next = nullptr)
            : element(el), next(next) {}
         ~ElementNode();
      };

      ElementNode* m_Elements;
      float m_Spacing;
      float m_FreeSlotPos;
   };
}

#endif
