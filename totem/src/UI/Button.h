#ifndef _TOTEM_UI_BUTTON_H_
#define _TOTEM_UI_BUTTON_H_

#include "UIElement.h"
#include "Animations/Animator.h"
#include "Animations/BasicAnimations.h"

namespace totem
{

   enum class ButtonType
   { 
      BoxButton
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

   
   class IClickListener
   {
   public:
      virtual ~IClickListener() {}
      virtual void OnClick() = 0;
   };

   class Button : public UIElement
   {
   public:
      Button(UIManager* manager, IClickListener* listener)
         :  UIElement(manager),
            m_Text(nullptr),
            m_State(State::Idle),
            m_Listener(listener)
      {}
      virtual ~Button();

      virtual void OnEvent(Event& e) override;
      
      virtual void SetPos(const math::vec2f& pos);
      virtual void SetScale(const math::vec2f& scale);
      virtual void SetColor(const math::vec4f& color);
      virtual void SetText(const char* str);

   private:
      virtual bool IsCursorHovered(const math::vec2i& cursorCoords) const;

   protected:
      virtual void OnIdle() {}
      virtual void OnHover() {}
      virtual void OnPush() {} 

   protected:
      enum class State { Idle, Hovered, Pushed };
      math::vec2f m_Pos;
      math::vec2f m_Scale;
      char* m_Text;

      State m_State;
      IClickListener* m_Listener;
   };

   class BoxButton : public Button
   {
   public:
      BoxButton(UIManager* manager, IClickListener* m_Listener);

   private:
      virtual void OnHover() override;
      virtual void OnIdle() override;
      virtual void OnPush() override;

      virtual void OnUpdate(float deltaTime) override;
      virtual void Draw() override;

      virtual void SetScale(const math::vec2f& scale) override; 
      virtual void SetColor(const math::vec4f& color) override;

   private:
      math::vec4f m_Color;
      Animator m_Animator;
      AnimationGroup m_IdleAnim;
      AnimationGroup m_HoverAnim;
      Animation* m_PushAnim;
      AnimationGroup m_AnimGroup;
      math::vec2f m_InitScale;
      InterpAnim<math::vec2f>* m_HoverScaleAnim;
      InterpAnim<math::vec2f>* m_IdleScaleAnim;
   };
}

#endif
