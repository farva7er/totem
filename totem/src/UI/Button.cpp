#include <string.h>

#include "Button.h"

namespace totem
{
////////////////////////////////////////////////////////////////
//      BaseButton         /////////////////////////////////////
////////////////////////////////////////////////////////////////

   BaseButton::~BaseButton()
   {
      while(m_Listeners)
      {
         ListenerNode* savedNode = m_Listeners;
         m_Listeners = m_Listeners->m_Next;
         delete savedNode;
      }

      delete [] m_Text;
   }

   void BaseButton::OnEvent(Event& e)
   {  
      if(e.GetType() == EventType::MouseMove)
      {
         MouseMoveEvent& me = e.Cast<MouseMoveEvent>();
         math::vec2f mouseCoords(me.GetX(), me.GetY());
         bool isHovered = IsHovered(mouseCoords);

         if(m_State == State::Idle && isHovered)
         {
            m_State = State::Hovered;
            SendOnHover();
         }

         if(m_State == State::Hovered && !isHovered)
         {
            m_State = State::Idle;
            SendOnIdle();
         }

         if(m_State == State::Pushed && !isHovered)
         {
            m_State = State::Idle;
            SendOnIdle();
         }
      }

      if(e.GetType() == EventType::MouseReleased)
      {
         MouseReleasedEvent& me = e.Cast<MouseReleasedEvent>();
         if(me.GetButton() == 0 && m_State == State::Pushed)
         {
            m_State = State::Hovered;
            SendOnClick();
            SendOnHover();
         }
      }

      if(e.GetType() == EventType::MousePressed)
      {
         MousePressedEvent& me = e.Cast<MousePressedEvent>();
         if(me.GetButton() == 0 && m_State == State::Hovered)
         {
            m_State = State::Pushed;
            SendOnPush();
         }
      }
   }

   void BaseButton::SetPos(const math::vec2f& pos)
   {
      m_Pos = pos;
   }

   void BaseButton::SetScale(const math::vec2f& scale)
   {
      m_Scale = scale;
   }

   void BaseButton::SetText(const char* text)
   {
      if(m_Text)
         delete [] m_Text;
      m_Text = new char[strlen(text) + 1];
      strcpy(m_Text, text);
   }

   void BaseButton::SetColor(const math::vec4f& color)
   {
      m_Color = color;
   }


   void BaseButton::AddListener(IButtonListener* listener) 
   {
      m_Listeners = new ListenerNode(listener, m_Listeners);
   }

   void BaseButton::SendOnIdle()
   {
      ListenerNode* curr = m_Listeners;
      while(curr)
      {
         curr->m_Listener->OnIdle();
         curr = curr->m_Next;
      }
   }

   void BaseButton::SendOnHover()
   {
      ListenerNode* curr = m_Listeners;
      while(curr)
      {
         curr->m_Listener->OnHover();
         curr = curr->m_Next;
      }
   }

   void BaseButton::SendOnPush()
   {
      ListenerNode* curr = m_Listeners;
      while(curr)
      {
         curr->m_Listener->OnPush();
         curr = curr->m_Next;
      }
   }

   void BaseButton::SendOnClick()
   {
      ListenerNode* curr = m_Listeners;
      while(curr)
      {
         curr->m_Listener->OnClick();
         curr = curr->m_Next;
      }
   }

   ////////////////////////////////////////////////////////////////////
   /////         BoxButton       //////////////////////////////////////
   ////////////////////////////////////////////////////////////////////

   void BoxButton::Draw(Renderer* renderer) const
   {
      Rect rect = Rect::Builder()
                        .SetPos(m_Pos)
                        .SetScale(m_Scale)
                        .SetColor(m_Color)
                        .Construct();

      renderer->DrawRect(rect);
      if(m_Text)
      { 
         renderer->DrawControlledText(
                     m_Text, m_Pos, m_Scale, m_Scale.y * 0.7f,
                     math::vec4f(1, 1, 1, 1),
                     TextAlign::VCenter | TextAlign::HCenter
                     );
      }
   }

   bool BoxButton::IsHovered(const math::vec2f& cursorCoords) const
   {
      if(cursorCoords.x >= m_Pos.x - m_Scale.x &&
         cursorCoords.x <= m_Pos.x + m_Scale.x &&
         cursorCoords.y >= m_Pos.y - m_Scale.y &&
         cursorCoords.y <= m_Pos.y + m_Scale.y)
      {
         return true;
      }
      return false;
   }

//////////////////////////////////////////////////////////////
////////         ButtonBaseDecorator      ////////////////////
//////////////////////////////////////////////////////////////

   void ButtonBaseDecorator::Draw(Renderer* renderer) const
   {
      m_Wrapee->Draw(renderer);
   }

   void ButtonBaseDecorator::AddListener(IButtonListener* listener)
   {
      m_Wrapee->AddListener(listener);
   }

//////////////////////////////////////////////////////////////
////////       ButtonAnimDecorator     ///////////////////////
//////////////////////////////////////////////////////////////

   ButtonAnimDecorator::ButtonAnimDecorator(IButton* button)
      : ButtonBaseDecorator(button)
   {
      m_BaseScale = button->GetScale();
      m_BaseColor = button->GetColor();


      m_IdleAnimDuration = 0.3f;
      m_HoverAnimDuration = 0.2f;
      m_PushAnimDuration = 0.1f;

      m_HoverScaleFactor = 1.1f;
      m_PushScaleFactor = 1.05f;

      m_IdleColorAlpha = 0.7f;
      m_HoverColorAlpha = 0.9f;
      m_PushColorAlpha = 1.0f;

      m_IdleScaleAnim = new HermiteInterpAnim<math::vec2f>(
                                 math::vec2f(0, 0),
                                 m_BaseScale,
                                 m_IdleAnimDuration);

      m_HoverScaleAnim = new HermiteInterpAnim<math::vec2f>(
                                 math::vec2f(0, 0),
                                 m_BaseScale * m_HoverScaleFactor,
                                 m_HoverAnimDuration);

      m_PushScaleAnim = new HermiteInterpAnim<math::vec2f>(
                                 math::vec2f(0, 0),
                                 m_BaseScale * m_PushScaleFactor,
                                 m_PushAnimDuration);



      m_IdleColorAnim = new HermiteInterpAnim<math::vec4f>(
                                 math::vec4f(0, 0, 0, 0),
                                 math::vec4f(m_BaseColor.x,
                                             m_BaseColor.y,
                                             m_BaseColor.z,
                                             m_IdleColorAlpha),
                                 m_IdleAnimDuration);

      m_HoverColorAnim = new HermiteInterpAnim<math::vec4f>(
                                 math::vec4f(0, 0, 0, 0),
                                 math::vec4f(m_BaseColor.x,
                                             m_BaseColor.y,
                                             m_BaseColor.z,
                                             m_HoverColorAlpha),
                                 m_HoverAnimDuration);

      m_PushColorAnim = new HermiteInterpAnim<math::vec4f>(
                                 math::vec4f(0, 0, 0, 0),
                                 math::vec4f(m_BaseColor.x,
                                             m_BaseColor.y,
                                             m_BaseColor.z,
                                             m_PushColorAlpha),
                                 m_PushAnimDuration);

      m_AllAnimations.Add(m_IdleScaleAnim);
      m_AllAnimations.Add(m_HoverScaleAnim);
      m_AllAnimations.Add(m_PushScaleAnim);

      m_AllAnimations.Add(m_IdleColorAnim);
      m_AllAnimations.Add(m_HoverColorAnim);
      m_AllAnimations.Add(m_PushColorAnim);

      m_Animator.Add(m_AllAnimations);

      button->AddListener(new ButtonListener(this));
   }

   void ButtonAnimDecorator::OnUpdate(float deltaTime)
   {
      m_Animator.OnUpdate(deltaTime);
      math::vec2f scale;
      m_IdleScaleAnim->ApplyVal(scale);
      m_HoverScaleAnim->ApplyVal(scale);
      m_PushScaleAnim->ApplyVal(scale);

      if(!m_IdleScaleAnim->IsPaused() || !m_HoverScaleAnim->IsPaused() ||
         !m_PushScaleAnim->IsPaused())
      {
         ButtonBaseDecorator::SetScale(scale);
      }

      math::vec4f color;

      m_IdleColorAnim->ApplyVal(color);
      m_HoverColorAnim->ApplyVal(color);
      m_PushColorAnim->ApplyVal(color);

      if(!m_IdleColorAnim->IsPaused() || !m_HoverColorAnim->IsPaused() ||
         !m_PushColorAnim->IsPaused())
      {
         ButtonBaseDecorator::SetColor(color);
      }
   }

   void ButtonAnimDecorator::SetScale(const math::vec2f& scale)
   {
      ButtonBaseDecorator::SetScale(scale);
      m_BaseScale = scale;
      m_IdleScaleAnim->SetFinVal(scale);
      m_HoverScaleAnim->SetFinVal(scale * m_HoverScaleFactor);
      m_PushScaleAnim->SetFinVal(scale * m_PushScaleFactor);
   }

   void ButtonAnimDecorator::SetColor(const math::vec4f& color)
   { 
      ButtonBaseDecorator::SetColor(
            math::vec4f(color.x, color.y, color.z, m_IdleColorAlpha));
      m_BaseColor = color;
      m_IdleColorAnim->SetFinVal(math::vec4f(color.x,
                                             color.y,
                                             color.z,
                                             m_IdleColorAlpha));

      m_HoverColorAnim->SetFinVal(math::vec4f(color.x,
                                             color.y,
                                             color.z,
                                             m_HoverColorAlpha));

      m_PushColorAnim->SetFinVal(math::vec4f(color.x,
                                             color.y,
                                             color.z,
                                             m_PushColorAlpha));
   }

   void ButtonAnimDecorator::OnIdle()
   {
      m_AllAnimations.Pause();

      m_IdleScaleAnim->SetInitVal(GetScale());
      m_IdleScaleAnim->Reset();
      m_IdleScaleAnim->Play();

      m_IdleColorAnim->SetInitVal(GetColor());
      m_IdleColorAnim->Reset();
      m_IdleColorAnim->Play();
   }

   void ButtonAnimDecorator::OnHover()
   {
      m_AllAnimations.Pause();

      m_HoverScaleAnim->SetInitVal(GetScale());
      m_HoverScaleAnim->Reset();
      m_HoverScaleAnim->Play();

      m_HoverColorAnim->SetInitVal(GetColor());
      m_HoverColorAnim->Reset();
      m_HoverColorAnim->Play();
   }

   void ButtonAnimDecorator::OnPush()
   {
      m_AllAnimations.Pause();

      m_PushScaleAnim->SetInitVal(GetScale());
      m_PushScaleAnim->Reset();
      m_PushScaleAnim->Play();

      m_PushColorAnim->SetInitVal(GetColor());
      m_PushColorAnim->Reset();
      m_PushColorAnim->Play();
   }

   void ButtonAnimDecorator::ButtonListener::OnIdle()
   {
      m_Master->OnIdle();
   }

   void ButtonAnimDecorator::ButtonListener::OnHover()
   {
      m_Master->OnHover();
   }

   void ButtonAnimDecorator::ButtonListener::OnPush()
   {
      m_Master->OnPush();
   } 
}
