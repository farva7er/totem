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
      EventDispatcher<BaseElement> d(this);
      d.Dispatch<MouseMoveEvent>(&BaseElement::OnMouseMove, e);
      d.Dispatch<MousePressedEvent>(&BaseElement::OnMousePressed, e);
      d.Dispatch<MouseReleasedEvent>(&BaseElement::OnMouseReleased, e);
   }

   void BaseElement::OnMouseMove(MouseMoveEvent& e)
   {
      math::vec2f mouseCoords(e.GetX(), e.GetY());
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

   void BaseElement::OnMousePressed(MousePressedEvent& e)
   {
      if(m_State == State::Hovered)
      {
         m_State = State::Pushed;
         SendOnPush();
      }
   }

   void BaseElement::OnMouseReleased(MouseReleasedEvent& e)
   {
      if(m_State == State::Pushed)
      {
         m_State = State::Hovered;
         SendOnClick(e.GetButton());
         SendOnHover();
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
