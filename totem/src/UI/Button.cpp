#include <string.h>

#include "Button.h"

namespace totem
{

   Button::~Button()
   {
      delete m_Listener;
      delete [] m_Text;
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

   void Button::SetColor(const math::vec4f& color)
   {}

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
/*
      Animation* hoverColorAnim = new totem::HermiteModifAnim<math::vec4f>(
                              totem::math::vec4f()
                              totem::math::vec4f(0, 0, 0, 0.8f),
                              0.2f
                           );
      m_HoverScaleAnim = new totem::HermiteModifAnim<math::vec2f>(
                              1.1f * m_InitScale,
                              0.2f
                           );

      m_HoverAnim.Add(hoverColorAnim);
      m_HoverAnim.Add(m_HoverScaleAnim);

      Animation* idleColorAnim = new totem::HermiteModifAnim<math::vec4f>(
                        m_Color,
                        totem::math::vec4f(0, 0, 0, 0.5f),
                        0.5f
                     );

      m_IdleScaleAnim =  new totem::HermiteModifAnim<math::vec2f>(
                        m_Scale,
                        m_InitScale,
                        0.2f
                     );

      m_IdleAnim.Add(idleColorAnim);
      m_IdleAnim.Add(m_IdleScaleAnim);

      m_PushAnim = new totem::HermiteModifAnim<math::vec4f>(
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
*/
   }

   void BoxButton::OnHover()
   {
      //m_AnimGroup.Pause();
      //m_HoverAnim.Play();
   }

   void BoxButton::OnIdle()
   {
      //m_AnimGroup.Pause();
      //m_IdleAnim.Play();
   }

   void BoxButton::OnPush()
   {
      //m_Animator.Pause(m_AnimGroup);
      //m_Animator.Play(m_PushAnim);
   }

   void BoxButton::OnUpdate(float deltaTime)
   {
      //m_Animator.OnUpdate(deltaTime);
   }

   void BoxButton::Draw()
   {
      Renderer* r = GetRenderer();
      //LOG_INFO("%f %f", m_InitScale.x, m_Scale.x);
      Rect rect = Rect::Builder()
                        .SetPos(m_Pos)
                        .SetScale(m_Scale)
                        .SetColor(m_Color)
                        .Construct();
      r->DrawRect(rect);
      if(m_Text)
      { 
         r->DrawControlledText(
                     m_Text, m_Pos, m_Scale, m_Scale.y * 0.7f,
                     math::vec4f(1, 1, 1, 1),
                     TextAlign::VCenter | TextAlign::HCenter
                     );
      }
   }


   void BoxButton::SetScale(const math::vec2f& scale)
   {
      Button::SetScale(scale);
      //m_HoverScaleAnim->SetFinVal(1.1 * m_InitScale);
      //m_IdleScaleAnim->SetFinVal(m_InitScale);
   }

   void BoxButton::SetColor(const math::vec4f& color)
   {
      m_Color = color;
   }

}
