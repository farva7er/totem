#ifndef _TOTEM_UI_BUTTON_H_
#define _TOTEM_UI_BUTTON_H_

#include "IUIElement.h"
#include "BaseElement.h"
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

   // In order to use a button
   // client should create a class that implements
   // IClickListener, then using a factory method
   // in UIManager create a button, passing the IClickListener
   // as a parameter to factory method
   //
   // For instance, in order to create a button using CreateButton method
   // create a class that implements IClickListener inteface.
   // This class can hold any data that will be necessary when button
   // is clicked.
   //
   // 
   // class ExampleButtonListener : public totem::IClickListener
   // {
   // public:
   //    ExampleButtonListener(SomeType* someData)
   //       : m_SomeData(someData) {}
   //
   //    void OnClick()
   //    {
   //       Do necessary actions here...
   //    }
   // private:
   //    SomeType* m_SomeData;
   // };
   //
   // After that you can create a button like that
   //
   // Button* exampleButton = m_UIManager.CreateButton(
   //                            totem::ButtonType::BoxButton,
   //                            new ExampleButtonListener(someData));

   

   class IButton : public IUIElement
   {
   public:
      virtual const char* GetText() const = 0;
      virtual math::vec4f GetColor() const = 0;
      virtual void SetText(const char* text) = 0;
      virtual void SetColor(const math::vec4f& color) = 0;
      virtual void AddListener(IUIElementListener* listener) = 0;
   };

   
   class BoxButton : public BaseElement, public IButton
   {
   public:
      BoxButton();

      virtual void Draw(Renderer* renderer) const override;
      virtual void OnEvent(Event& e) override
      { BaseElement::OnEvent(e); }
      
      virtual const math::vec2f& GetPos() const override
      { return BaseElement::GetPos(); }

      virtual const math::vec2f& GetScale() const override
      { return BaseElement::GetScale(); }

      virtual void SetPos(const math::vec2f& pos) override
      { BaseElement::SetPos(pos); }

      virtual void SetScale(const math::vec2f& scale) override
      { BaseElement::SetScale(scale); }

      virtual void AddListener(IUIElementListener* listener) override
      { BaseElement::AddListener(listener); }

      virtual const char* GetText() const override
      { return m_Text; }

      virtual math::vec4f GetColor() const override
      { return m_Color; }

      virtual void SetText(const char* text) override;
      virtual void SetColor(const math::vec4f& color) override
      { m_Color = color; }

   private:
      virtual bool IsHovered(const math::vec2f& cursorCoords) const override;
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

      virtual void AddListener(IUIElementListener* listener) override;

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
      class ButtonListener : public IUIElementListener
      {
      public:
         ButtonListener(ButtonScaleDecorator* master)
            : m_Master(master) {}

         virtual void OnLostHover() override;
         virtual void OnHover() override;
         virtual void OnPush() override;

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
      class ButtonListener : public IUIElementListener
      {
      public:
         ButtonListener(ButtonColorDecorator* master)
            : m_Master(master) {}

         virtual void OnLostHover() override;
         virtual void OnHover() override;
         virtual void OnPush() override;

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
