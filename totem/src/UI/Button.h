#ifndef _TOTEM_UI_BUTTON_H_
#define _TOTEM_UI_BUTTON_H_

#include "UIElement.h"
#include "Animations/Animator.h"
#include "Animations/BasicAnimations.h"

namespace totem
{

   enum class ButtonType
   { 
      BoxButton,
      AnimatedBoxButton
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

   
   class IButtonListener
   {
   public:
      virtual ~IButtonListener() {}

      virtual void OnClick() {}
      virtual void OnIdle() {}
      virtual void OnHover() {}
      virtual void OnPush() {}
   };

   class IButton : public UIElement
   {
   public:
      virtual math::vec2f GetPos() const = 0;
      virtual math::vec2f GetScale() const = 0;
      virtual math::vec4f GetColor() const = 0;
      virtual const char* GetText() const = 0;

      virtual void SetPos(const math::vec2f& pos) = 0;
      virtual void SetScale(const math::vec2f& scale) = 0;
      virtual void SetColor(const math::vec4f& color) = 0;
      virtual void SetText(const char* str) = 0;

      virtual void AddListener(IButtonListener* listener) = 0;

   protected:
      enum class State { Idle, Hovered, Pushed };

   };

   class BaseButton : public IButton
   {
   public:
      BaseButton()
         :  m_Pos(math::vec2f(0, 0)),
            m_Scale(math::vec2f(1, 1)),
            m_Color(math::vec4f(0, 0, 0, 0.5f)),
            m_Text(nullptr),
            m_State(State::Idle),
            m_Listeners(nullptr)
      {}

      virtual ~BaseButton();

      virtual void OnEvent(Event& e) override;
      
      virtual math::vec2f GetPos() const override { return m_Pos; }
      virtual math::vec2f GetScale() const override { return m_Scale; }
      virtual math::vec4f GetColor() const override { return m_Color; }
      virtual const char* GetText() const override { return m_Text; }

      virtual void SetPos(const math::vec2f& pos) override;
      virtual void SetScale(const math::vec2f& scale) override;
      virtual void SetColor(const math::vec4f& color) override;
      virtual void SetText(const char* str) override;

      virtual void AddListener(IButtonListener* listener) override;

   protected:
      virtual bool IsHovered(const math::vec2f& cursorCoords) const = 0;

   private:
      void SendOnIdle();
      void SendOnHover();
      void SendOnPush();
      void SendOnClick();

   protected:
      math::vec2f m_Pos;
      math::vec2f m_Scale;
      math::vec4f m_Color;
      char* m_Text;

      State m_State;

      struct ListenerNode
      {
         IButtonListener* m_Listener;
         ListenerNode* m_Next;
         ListenerNode(IButtonListener* listener,
                     ListenerNode* next = nullptr)
            : m_Listener(listener), m_Next(next)
         {}
         ~ListenerNode() { delete m_Listener; }
      };

      ListenerNode* m_Listeners;
   };

   class BoxButton : public BaseButton
   {
   public:
      virtual void Draw(Renderer* renderer) const override;

   private:
      virtual bool IsHovered(const math::vec2f& cursorCoords) const override;
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

      virtual math::vec2f GetPos() const override { return m_Wrapee->GetPos(); }
      virtual math::vec2f GetScale() const override { return m_Wrapee->GetScale(); }
      virtual math::vec4f GetColor() const override { return m_Wrapee->GetColor(); }
      virtual const char* GetText() const override { return m_Wrapee->GetText(); }


      virtual void SetPos(const math::vec2f& pos) override { m_Wrapee->SetPos(pos); }
      virtual void SetScale(const math::vec2f& scale) override { m_Wrapee->SetScale(scale); }
      virtual void SetColor(const math::vec4f& color) override { m_Wrapee->SetColor(color); }
      virtual void SetText(const char* str) override { m_Wrapee->SetText(str); }

      virtual void AddListener(IButtonListener* listener) override;

   private:
      IButton* m_Wrapee;
   };

   class ButtonAnimDecorator : public ButtonBaseDecorator
   {
   public:
      ButtonAnimDecorator(IButton* button);

      virtual void OnUpdate(float deltaTime) override;

      virtual void SetScale(const math::vec2f& scale) override;
      virtual void SetColor(const math::vec4f& color) override;

      void OnIdle();
      void OnHover();
      void OnPush();

   private:
      class ButtonListener : public IButtonListener
      {
      public:
         ButtonListener(ButtonAnimDecorator* master)
            : m_Master(master) {}

         virtual void OnIdle() override;
         virtual void OnHover() override;
         virtual void OnPush() override;

         ButtonAnimDecorator* m_Master;
      };

      InterpAnim<math::vec2f>* m_IdleScaleAnim;
      InterpAnim<math::vec2f>* m_HoverScaleAnim;
      InterpAnim<math::vec2f>* m_PushScaleAnim;

      InterpAnim<math::vec4f>* m_IdleColorAnim;
      InterpAnim<math::vec4f>* m_HoverColorAnim;
      InterpAnim<math::vec4f>* m_PushColorAnim;

      AnimationGroup m_AllAnimations;

      Animator m_Animator;

      math::vec2f m_BaseScale;
      math::vec4f m_BaseColor;

      float m_IdleAnimDuration;
      float m_HoverAnimDuration;
      float m_PushAnimDuration;

      float m_HoverScaleFactor;
      float m_PushScaleFactor;

      float m_IdleColorAlpha;
      float m_HoverColorAlpha;
      float m_PushColorAlpha;
   };
}

#endif
