#include <string.h>

#include "Button.h"

namespace totem
{

   Button::~Button()
   {
      delete m_Listener;
      delete m_Text;
   }

   void Button::OnEvent(Event& e)
   {
      if(e.GetType() == EventType::MouseMove)
      {
         MouseMoveEvent& me = e.Cast<MouseMoveEvent>();
         math::vec2i cursorCoords(me.GetX(), me.GetY());
         bool cursorHovered = IsCursorHovered(cursorCoords);

         if(m_State == State::Idle && cursorHovered)
         {
            m_State = State::Hovered;
            OnHover();
         }

         if(m_State == State::Hovered && !cursorHovered)
         {
            m_State = State::Idle;
            OnIdle();
         }

         if(m_State == State::Pushed && !cursorHovered)
         {
            m_State = State::Idle;
            OnIdle();
         }
      }

      if(e.GetType() == EventType::MouseReleased)
      {
         MouseReleasedEvent& me = e.Cast<MouseReleasedEvent>();
         if(me.GetButton() == 0 && m_State == State::Pushed)
         {
            m_State = State::Hovered;
            m_Listener->OnClick();
            OnHover();
         }
      }

      if(e.GetType() == EventType::MousePressed)
      {
         MousePressedEvent& me = e.Cast<MousePressedEvent>();
         if(me.GetButton() == 0 && m_State == State::Hovered)
         {
            m_State = State::Pushed;
            OnPush();
         }
      }
   }

   void Button::SetPos(const math::vec2f& pos)
   {
      m_Pos = pos;
   }

   void Button::SetScale(const math::vec2f& scale)
   {
      m_Scale = scale;
   }

   void Button::SetText(const char* text)
   {
      if(m_Text)
         delete [] m_Text;
      m_Text = new char[strlen(text) + 1];
      strcpy(m_Text, text);
   }

   bool Button::IsCursorHovered(const math::vec2i& cursorCoords) const
   {
      Renderer* renderer = GetRenderer();
      math::vec2f cursorCoordsNorm = renderer->ScreenToNormal(cursorCoords);
      float normScaleX = renderer->CamUnitXToNormal(m_Scale.x);
      float normScaleY = renderer->CamUnitYToNormal(m_Scale.y);

      //LOG_INFO("nx: %f, ny: %f", cursorCoordsNorm.x, cursorCoordsNorm.y);

      if(cursorCoordsNorm.x >= m_Pos.x - normScaleX &&
         cursorCoordsNorm.x <= m_Pos.x + normScaleX &&
         cursorCoordsNorm.y >= m_Pos.y - normScaleY &&
         cursorCoordsNorm.y <= m_Pos.y + normScaleY)
      {
         return true;
      }
      return false;
   }

   BoxButton::BoxButton(UIManager* manager, IClickListener* listener) 
      :  Button(manager, listener),
         m_Color(math::vec4f(0, 0, 0, 0.5f)) 
   {

      Animation* hoverColorAnim =   new totem::HermiteModifAnim(
                              m_Color,
                              totem::math::vec4f(0, 0, 0, 0.8f),
                              0.2f
                           );
      Animation* hoverScaleAnim =  new totem::HermiteModifAnim(
                              m_Scale,
                              totem::math::vec2f(2.2f, 1.2),
                              0.2f
                           );

      m_HoverAnim.Add(hoverColorAnim);
      m_HoverAnim.Add(hoverScaleAnim);

      Animation* idleColorAnim =  new totem::HermiteModifAnim(
                        m_Color,
                        totem::math::vec4f(0, 0, 0, 0.5f),
                        0.5f
                     );

      Animation* idleScaleAnim =  new totem::HermiteModifAnim(
                        m_Scale,
                        totem::math::vec2f(2, 1),
                        0.2f
                     );

      m_IdleAnim.Add(idleColorAnim);
      m_IdleAnim.Add(idleScaleAnim);

      m_PushAnim = new totem::HermiteModifAnim(
                        m_Color,
                        totem::math::vec4f(0, 0, 0, 1.0f),
                        0.1f
                     );

      m_Animator.Add(m_HoverAnim);
      m_Animator.Add(m_IdleAnim);
      m_Animator.Add(m_PushAnim);

      m_AnimGroup.Add(m_HoverAnim);
      m_AnimGroup.Add(m_IdleAnim);
      m_AnimGroup.Add(m_PushAnim);

   }

   void BoxButton::OnHover()
   {
      m_Animator.Pause(m_AnimGroup);
      m_Animator.Play(m_HoverAnim);
   }

   void BoxButton::OnIdle()
   {
      m_Animator.Pause(m_AnimGroup);
      m_Animator.Play(m_IdleAnim);
   }

   void BoxButton::OnPush()
   {
      m_Animator.Pause(m_AnimGroup);
      m_Animator.Play(m_PushAnim);
   }

   void BoxButton::OnUpdate(float deltaTime)
   {
      m_Animator.OnUpdate(deltaTime);
   }

   void BoxButton::Draw()
   {
      Renderer* r = GetRenderer();
      r->DrawRect(m_Pos, m_Scale, m_Color);
      if(m_Text)
      { 
         r->DrawControlledText(
                     m_Text, m_Pos, m_Scale, m_Scale.y * 0.7f,
                     math::vec4f(1, 1, 1, 1),
                     TextAlign::VCenter | TextAlign::HCenter
                     );
      }
   }
}
