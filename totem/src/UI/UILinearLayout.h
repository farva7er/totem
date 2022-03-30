#ifndef _TOTEM_UIMANAGER_H_
#define _TOTEM_UIMANAGER_H_

#include "Events.h"
#include "Renderer/Renderer.h"
#include "Button.h"
#include "IUIElement.h"

namespace totem
{   
   class UILinearLayout : public IUIElement
   {
     
   public:
      virtual ~UILinearLayout();

      virtual void OnEvent(Event& e) override;
      virtual void OnUpdate(float deltaTime) override;
      virtual void Draw(Renderer* renderer) override;

      virtual const math::vec2f& GetPos() const override { return m_Pos; }
      virtual const math::vec2f& GetScale() const override { return m_Scale; }

      virtual void SetPos(const math::vec2f& pos) override;
      virtual void SetScale(const math::vec2f& scale) override;

   private:
      void AddElement(IUIElement* element);
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
      math::vec2f m_Pos;
      math::vec2f m_Scale;
      float m_FreeSlotPos;
   };
}

#endif
