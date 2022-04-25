#ifndef _TOTEM_UIMANAGER_H_
#define _TOTEM_UIMANAGER_H_

#include "Events.h"
#include "Renderer/Renderer.h"
#include "IMovableElement.h"
#include "BasicElementImpl.h"

namespace totem
{   
   class UILinearLayout : public IMovableElement, public BasicElementImpl
   {
     
   public:
      UILinearLayout();
      virtual ~UILinearLayout();

      // Temporary
      UILinearLayout(const UILinearLayout& other) = delete;
      // Temporary
      UILinearLayout& operator=(const UILinearLayout& other) = delete;

      virtual void OnEvent(Event& e) override;
      virtual void OnUpdate(float deltaTime) override;
      virtual void Draw(Renderer* renderer) const override;

      virtual const math::vec2f& GetPos() const override
      { return BasicElementImpl::GetPos(); }

      virtual void SetPos(const math::vec2f& pos) override;

      virtual const math::vec2f& GetScale() const override
      { return BasicElementImpl::GetScale(); }

      virtual void SetScale(const math::vec2f& scale) override;

      void SetSpacing(float spacing);

      void AddElement(IMovableElement* element);

      virtual IUIElement* GetParent() const override
      { return BasicElementImpl::GetParent(); }

      virtual void SetParent(IUIElement* el) override
      { BasicElementImpl::SetParent(el); }

      virtual void Forget(unsigned int elID) override;

      virtual unsigned int GetID() const override
      { return BasicElementImpl::GetID(); }

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
