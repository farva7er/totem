#ifndef _TOTEM_UI_BUTTON_H_
#define _TOTEM_UI_BUTTON_H_

#include "IMovableElement.h"
#include "InteractiveElementImpl.h"
#include "Animations/Animator.h"
#include "Animations/BasicAnimations.h"

namespace totem
{
   enum class ButtonType
   { 
      SimpleBoxButton,
      FixedBoxButton,
      SoftBoxButton
   };

   class IButton : public IMovableElement
   {
   public:
      virtual ~IButton() = default;

      virtual bool IsActive() const = 0;
      virtual void SetActive(bool isActive) = 0;
 
      virtual const char* GetText() const = 0;
      virtual void SetText(const char* text) = 0;

      virtual math::vec4f GetColor() const = 0;
      virtual void SetColor(const math::vec4f& color) = 0;

      virtual void AddListener(IIEListener* listener) = 0;
      virtual void RemoveListener(IIEListener* listener) = 0;
   };

   
   class BoxButton : public IButton, public InteractiveElementImpl
   {
   public:
      BoxButton();
      virtual ~BoxButton();
      BoxButton(const BoxButton& other);
      BoxButton& operator=(const BoxButton& other);

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

      virtual void SetScale(const math::vec2f& scale) override
      { InteractiveElementImpl::SetScale(scale); }

      virtual void AddListener(IIEListener* listener) override
      { InteractiveElementImpl::AddListener(listener); }

      virtual void RemoveListener(IIEListener* listener) override
      { InteractiveElementImpl::RemoveListener(listener); }

      virtual const char* GetText() const override
      { return m_Text; }

      virtual math::vec4f GetColor() const override
      { return m_Color; }

      virtual void SetText(const char* text) override;
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

   private:
      char* m_Text;
      math::vec4f m_Color;
   };



////////////////////////////// decorator ///////////////////////////////

   class ButtonBaseDecorator : public IButton
   {
   public:
      ButtonBaseDecorator(IButton* button)
         : m_Wrapee(button) {}

      virtual ~ButtonBaseDecorator() { delete m_Wrapee; }

      virtual void Draw(Renderer* renderer) const override;
      virtual void OnEvent(Event& e) override { m_Wrapee->OnEvent(e); }

      virtual bool IsActive() const override
      { return m_Wrapee->IsActive(); }
      virtual void SetActive(bool isActive) override
      { m_Wrapee->SetActive(isActive); }

      virtual const math::vec2f& GetPos() const override
      { return m_Wrapee->GetPos(); }

      virtual const math::vec2f& GetScale() const override
      { return m_Wrapee->GetScale(); }

      virtual math::vec4f GetColor() const override
      { return m_Wrapee->GetColor(); }

      virtual const char* GetText() const override
      { return m_Wrapee->GetText(); }


      virtual void SetPos(const math::vec2f& pos) override
      { m_Wrapee->SetPos(pos); }

      virtual void SetScale(const math::vec2f& scale) override
      { m_Wrapee->SetScale(scale); }

      virtual void SetColor(const math::vec4f& color) override
      { m_Wrapee->SetColor(color); }

      virtual void SetText(const char* str) override
      { m_Wrapee->SetText(str); }

      virtual IUIElement* GetParent() const override
      { return m_Wrapee->GetParent(); }

      virtual void SetParent(IUIElement* el) override
      { m_Wrapee->SetParent(el); }

      virtual void Forget(unsigned int elID) override
      { m_Wrapee->Forget(elID); }

      virtual unsigned int GetID() const override
      { return m_Wrapee->GetID(); }

      virtual void AddListener(IIEListener* listener) override;

      virtual void RemoveListener(IIEListener* listener) override
      { m_Wrapee->RemoveListener(listener); }


   protected:
      IButton* m_Wrapee;
   };

   class ButtonScaleDecorator : public ButtonBaseDecorator
   {
   public:
      ButtonScaleDecorator(IButton* button);

      virtual void OnUpdate(float deltaTime) override;

      virtual void SetScale(const math::vec2f& scale) override;

      void OnLostHover();
      void OnHover();
      void OnPush();

   private:
      class ButtonListener : public IIEListener
      {
      public:
         ButtonListener(ButtonScaleDecorator* master)
            : m_Master(master) {}

         virtual ~ButtonListener() = default;
         ButtonListener(const ButtonListener& other) = default;
         ButtonListener& operator=(const ButtonListener& other) = default;

         virtual void OnLostHover() override;
         virtual void OnHover() override;
         virtual void OnPush() override;

         virtual IIEListener* Clone() const;
      private:
         ButtonScaleDecorator* m_Master;
      };

      InterpAnim<math::vec2f>* m_LostHoverScaleAnim;
      InterpAnim<math::vec2f>* m_HoverScaleAnim;
      InterpAnim<math::vec2f>* m_PushScaleAnim;

      AnimationGroup m_AllAnimations;

      Animator m_Animator;

      math::vec2f m_BaseScale;

      float m_LostHoverAnimDuration;
      float m_HoverAnimDuration;
      float m_PushAnimDuration;

      float m_HoverScaleFactor;
      float m_PushScaleFactor;
   };

   class ButtonColorDecorator : public ButtonBaseDecorator
   {
   public:
      ButtonColorDecorator(IButton* button);

      virtual void OnUpdate(float deltaTime) override;

      virtual void SetColor(const math::vec4f& color) override;

      void OnLostHover();
      void OnHover();
      void OnPush();

   private:
      class ButtonListener : public IIEListener
      {
      public:
         ButtonListener(ButtonColorDecorator* master)
            : m_Master(master) {}

         virtual ~ButtonListener() = default;
         ButtonListener(const ButtonListener& other) = default;
         ButtonListener& operator=(const ButtonListener& other) = default;


         virtual void OnLostHover() override;
         virtual void OnHover() override;
         virtual void OnPush() override;

         virtual IIEListener* Clone() const;
      private:
         ButtonColorDecorator* m_Master;
      };

      InterpAnim<math::vec4f>* m_LostHoverColorAnim;
      InterpAnim<math::vec4f>* m_HoverColorAnim;
      InterpAnim<math::vec4f>* m_PushColorAnim;

      AnimationGroup m_AllAnimations;

      Animator m_Animator;

      math::vec4f m_BaseColor;

      float m_LostHoverAnimDuration;
      float m_HoverAnimDuration;
      float m_PushAnimDuration;

      float m_LostHoverColorAlpha;
      float m_HoverColorAlpha;
      float m_PushColorAlpha;
   };

}

#endif
