#include "InteractiveElementImpl.h"
#include "Events.h"

namespace totem
{
   InteractiveElementImpl::~InteractiveElementImpl()
   {
      ClearListeners();
   }

   InteractiveElementImpl::
   InteractiveElementImpl(const InteractiveElementImpl& other)
      : BasicElementImpl(other)
   {
      m_State = State::LostHover;

      ListenerNode* currOther = other.m_Listeners;
      while(currOther)
      {
         AddListener(currOther->m_Listener->Clone());
         currOther = currOther->m_Next;
      }
   }

   InteractiveElementImpl&
   InteractiveElementImpl::operator=(const InteractiveElementImpl& other)
   {
      BasicElementImpl::operator=(other);
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

   void InteractiveElementImpl::ClearListeners()
   {
      while(m_Listeners)
      {
         ListenerNode* savedNode = m_Listeners;
         m_Listeners = m_Listeners->m_Next;
         delete savedNode;
      }

      m_Listeners = nullptr;
   }

   void InteractiveElementImpl::SetActive(bool isActive)
   {
      m_IsActive = isActive;

      if(!isActive)
      {
         m_State = State::LostHover;
         SendOnLostHover();
      }
   }

   void InteractiveElementImpl::OnEvent(Event& e)
   {
      if(!m_IsActive)
         return;

      EventDispatcher<InteractiveElementImpl> d(this);
      d.Dispatch<MouseMoveEvent>(
            &InteractiveElementImpl::OnMouseMove, e);
      d.Dispatch<MousePressedEvent>(
            &InteractiveElementImpl::OnMousePressed, e);
      d.Dispatch<MouseReleasedEvent>(
            &InteractiveElementImpl::OnMouseReleased, e);
   }

   void InteractiveElementImpl::OnMouseMove(MouseMoveEvent& e)
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

   void InteractiveElementImpl::OnMousePressed(MousePressedEvent& /*e*/)
   {
      if(m_State == State::Hovered)
      {
         m_State = State::Pushed;
         SendOnPush();
      }
   }

   void InteractiveElementImpl::OnMouseReleased(MouseReleasedEvent& e)
   {
      if(m_State == State::Pushed)
      {
         m_State = State::Hovered;
         SendOnClick(e.GetButton());
         SendOnHover();
      }
   }

   void InteractiveElementImpl::AddListener(IIEListener* listener) 
   {
      m_Listeners = new ListenerNode(listener, m_Listeners);
   }

   void InteractiveElementImpl::SendOnLostHover()
   {
      ListenerNode* curr = m_Listeners;
      while(curr)
      {
         curr->m_Listener->OnLostHover();
         curr = curr->m_Next;
      }
      OnLostHover();
   }

   void InteractiveElementImpl::SendOnHover()
   {
      ListenerNode* curr = m_Listeners;
      while(curr)
      {
         curr->m_Listener->OnHover();
         curr = curr->m_Next;
      }
      OnHover();
   }

   void InteractiveElementImpl::SendOnPush()
   {
      ListenerNode* curr = m_Listeners;
      while(curr)
      {
         curr->m_Listener->OnPush();
         curr = curr->m_Next;
      }
      OnPush();
   }

   void InteractiveElementImpl::SendOnClick(int button)
   {
      ListenerNode* curr = m_Listeners;
      while(curr)
      {
         curr->m_Listener->OnClick(button);
         curr = curr->m_Next;
      }
      OnClick(button);
   }

   bool
   InteractiveElementImpl::IsHovered(const math::vec2f& cursorCoords) const
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
