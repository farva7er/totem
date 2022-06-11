#ifndef _TOTEM_CHECK_BUTTON_H_
#define _TOTEM_CHECK_BUTTON_H_

#include "Button.h"

namespace totem
{
   class CheckButton : public IButton, public InteractiveElementImpl
   {
   public:
      CheckButton();
      virtual ~CheckButton() = default;
      CheckButton(const CheckButton& other);
      CheckButton& operator=(const CheckButton& other);

      virtual void Draw(Renderer* renderer) const override;
      virtual void OnEvent(Event& e) override
      { InteractiveElementImpl::OnEvent(e); }

      virtual bool IsActive() const override
      { return InteractiveElementImpl::IsActive(); }

      virtual void SetActive(bool isActive) override
      { InteractiveElementImpl::SetActive(isActive); }
      
      virtual const math::vec2f& GetPos() const override
      { return InteractiveElementImpl::GetPos(); }

      virtual const math::vec2f& GetScale() const override
      { return InteractiveElementImpl::GetScale(); }

      virtual void SetPos(const math::vec2f& pos) override
      { InteractiveElementImpl::SetPos(pos); }

      virtual void SetScale(const math::vec2f& scale) override;

      virtual void AddListener(IIEListener* listener) override
      { InteractiveElementImpl::AddListener(listener); }

      virtual void RemoveListener(IIEListener* listener) override
      { InteractiveElementImpl::RemoveListener(listener); }

      virtual math::vec4f GetColor() const override
      { return m_Color; }

      virtual void SetColor(const math::vec4f& color) override
      { m_Color = color; }

      virtual IUIElement* GetParent() const override
      { return InteractiveElementImpl::GetParent(); }

      virtual void SetParent(IUIElement* el) override
      { InteractiveElementImpl::SetParent(el); }

      virtual void Forget(unsigned int elID) override
      { InteractiveElementImpl::Forget(elID); }

      virtual unsigned int GetID() const override
      { return InteractiveElementImpl::GetID(); }

      virtual Text GetText() const override
      { return Text(); }

      virtual void SetText(const Text& /*text*/) override {}

      bool IsChecked() const;
      void SetChecked(bool checked);

   private:
      virtual void OnClick(int button) override;

   private:
      bool m_IsChecked;
      math::vec4f m_Color;
   };
}

#endif
