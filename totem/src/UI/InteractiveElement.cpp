#include "InteractiveElement.h"
#include "Events.h"

namespace totem
{
   InteractiveElement::~InteractiveElement()
   {
      ClearListeners();
   }

   InteractiveElement::InteractiveElement(const InteractiveElement& other)
      : MovableElement(other)
   {
      m_State = State::LostHover;

      ListenerNode* currOther = other.m_Listeners;
      while(currOther)
      {
         AddListener(currOther->m_Listener->Clone());
         currOther = currOther->m_Next;
      }
   }

   InteractiveElement&
   InteractiveElement::operator=(const InteractiveElement& other)
   {
      MovableElement::operator=(other);
      m_State = State::LostHover;
      
      ClearListeners();
      ListenerNode* currOther = other.m_Listeners;
      while(currOther)
      {
         AddListener(currOther->m_Listener->Clone());
         currOther = currOther->m_Next;
      }
      return *this;
   }

   void InteractiveElement::ClearListeners()
   {
      while(m_Listeners)
      {
         ListenerNode* savedNode = m_Listeners;
         m_Listeners = m_Listeners->m_Next;
         delete savedNode;
      }

      m_Listeners = nullptr;
   }

   void InteractiveElement::OnEvent(Event& e)
   {
      EventDispatcher<InteractiveElement> d(this);
      d.Dispatch<MouseMoveEvent>(&InteractiveElement::OnMouseMove, e);
      d.Dispatch<MousePressedEvent>(&InteractiveElement::OnMousePressed, e);
      d.Dispatch<MouseReleasedEvent>(&InteractiveElement::OnMouseReleased, e);
   }

   void InteractiveElement::OnMouseMove(MouseMoveEvent& e)
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

   void InteractiveElement::OnMousePressed(MousePressedEvent& e)
   {
      if(m_State == State::Hovered)
      {
         m_State = State::Pushed;
         SendOnPush();
      }
   }

   void InteractiveElement::OnMouseReleased(MouseReleasedEvent& e)
   {
      if(m_State == State::Pushed)
      {
         m_State = State::Hovered;
         SendOnClick(e.GetButton());
         SendOnHover();
      }
   }

   void InteractiveElement::AddListener(IIEListener* listener) 
   {
      m_Listeners = new ListenerNode(listener, m_Listeners);
   }

   void InteractiveElement::SendOnLostHover()
   {
      ListenerNode* curr = m_Listeners;
      while(curr)
      {
         curr->m_Listener->OnLostHover();
         curr = curr->m_Next;
      }
   }

   void InteractiveElement::SendOnHover()
   {
      ListenerNode* curr = m_Listeners;
      while(curr)
      {
         curr->m_Listener->OnHover();
         curr = curr->m_Next;
      }
   }

   void InteractiveElement::SendOnPush()
   {
      ListenerNode* curr = m_Listeners;
      while(curr)
      {
         curr->m_Listener->OnPush();
         curr = curr->m_Next;
      }
   }

   void InteractiveElement::SendOnClick(int button)
   {
      ListenerNode* curr = m_Listeners;
      while(curr)
      {
         curr->m_Listener->OnClick(button);
         curr = curr->m_Next;
      }
   }


   bool InteractiveElement::IsHovered(const math::vec2f& cursorCoords) const
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
}
