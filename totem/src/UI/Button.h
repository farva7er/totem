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

   class IClickListener
   {
   public:
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

      virtual void OnEvent(Event& e) override;
      
      void SetPos(const math::vec2f& pos);
      void SetScale(const math::vec2f& scale);
      void SetText(const char* str);

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
   
   private:
      math::vec4f m_Color;
      Animator m_Animator;
      AnimationGroup m_IdleAnim;
      AnimationGroup m_HoverAnim;
      Animation* m_PushAnim;
      AnimationGroup m_AnimGroup;
   };
}

#endif
