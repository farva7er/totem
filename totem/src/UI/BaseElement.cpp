#include "BaseElement.h"

namespace totem
{
////////////////////////////////////////////////////////////////
//      BaseElement         ////////////////////////////////////
////////////////////////////////////////////////////////////////

   BaseElement::~BaseElement()
   {
      while(m_Listeners)
      {
         ListenerNode* savedNode = m_Listeners;
         m_Listeners = m_Listeners->m_Next;
         delete savedNode;
      }
   }

   void BaseElement::OnEvent(Event& e)
   {  
      if(e.GetType() == EventType::MouseMove)
      {
         MouseMoveEvent& me = e.Cast<MouseMoveEvent>();
         math::vec2f mouseCoords(me.GetX(), me.GetY());
         bool isHovered = IsHovered(mouseCoords);

         if(m_State == State::LostHover && isHovered)
         {
            m_State = State::Hovered;
            SendOnHover();
         }

         if(m_State == State::Hovered && !isHovered)
         {
            m_State = State::LostHover;
            SendOnLostHover();
         }

         if(m_State == State::Pushed && !isHovered)
         {
            m_State = State::LostHover;
            SendOnLostHover();
         }
      }

      if(e.GetType() == EventType::MouseReleased)
      {
         MouseReleasedEvent& me = e.Cast<MouseReleasedEvent>();
         if(m_State == State::Pushed)
         {
            m_State = State::Hovered;
            SendOnClick(me.GetButton());
            SendOnHover();
         }
      }

      if(e.GetType() == EventType::MousePressed)
      {
         if(m_State == State::Hovered)
         {
            m_State = State::Pushed;
            SendOnPush();
         }
      }
   }

   void BaseElement::SetPos(const math::vec2f& pos)
   {
      m_Pos = pos;
   }

   void BaseElement::SetScale(const math::vec2f& scale)
   {
      m_Scale = scale;
   }

   void BaseElement::AddListener(IUIElementListener* listener) 
   {
      m_Listeners = new ListenerNode(listener, m_Listeners);
   }

   void BaseElement::SendOnLostHover()
   {
      ListenerNode* curr = m_Listeners;
      while(curr)
      {
         curr->m_Listener->OnLostHover();
         curr = curr->m_Next;
      }
   }

   void BaseElement::SendOnHover()
   {
      ListenerNode* curr = m_Listeners;
      while(curr)
      {
         curr->m_Listener->OnHover();
         curr = curr->m_Next;
      }
   }

   void BaseElement::SendOnPush()
   {
      ListenerNode* curr = m_Listeners;
      while(curr)
      {
         curr->m_Listener->OnPush();
         curr = curr->m_Next;
      }
   }

   void BaseElement::SendOnClick(int button)
   {
      ListenerNode* curr = m_Listeners;
      while(curr)
      {
         curr->m_Listener->OnClick(button);
         curr = curr->m_Next;
      }
   }
}
