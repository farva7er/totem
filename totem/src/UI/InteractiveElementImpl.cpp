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
      m_State = LostHoverState::GetInstance();

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
      m_State = LostHoverState::GetInstance();
      
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
         m_State = LostHoverState::GetInstance();
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
      m_State->OnMouseMove(this, e);
   }

   void InteractiveElementImpl::OnMousePressed(MousePressedEvent& e)
   {
      m_State->OnMousePressed(this, e);
   }

   void InteractiveElementImpl::OnMouseReleased(MouseReleasedEvent& e)
   {
      m_State->OnMouseReleased(this, e);
   }

   void InteractiveElementImpl::SetState(State* newState)
   {
      m_State = newState;
   }

   void InteractiveElementImpl::AddListener(IIEListener* listener) 
   {
      m_Listeners = new ListenerNode(listener, m_Listeners);
   }

   void InteractiveElementImpl::RemoveListener(IIEListener* listener) 
   {
      ListenerNode** curr = &m_Listeners;
      while(*curr)
      {
         if((*curr)->m_Listener == listener)
         {
            ListenerNode* savedNode = *curr;
            (*curr)->m_Listener = nullptr;   // Client just wants us to
                                             // forget about this listener
                                             // it's not our responsibility
                                             // to delete listener
            *curr = (*curr)->m_Next;
            delete savedNode;
            return;
         }
         curr = &((*curr)->m_Next);
      }

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

   InteractiveElementImpl::LostHoverState
   InteractiveElementImpl::LostHoverState::s_Instance;

   InteractiveElementImpl::HoveredState
   InteractiveElementImpl::HoveredState::s_Instance;

   InteractiveElementImpl::PushedState
   InteractiveElementImpl::PushedState::s_Instance;

   void InteractiveElementImpl::State::
   ChangeState(InteractiveElementImpl* el, State* newState)
   {
      el->SetState(newState);
   }

   void InteractiveElementImpl::LostHoverState::
   OnMouseMove(InteractiveElementImpl* el, MouseMoveEvent& e)
   {
      math::vec2f mouseCoords(e.GetX(), e.GetY());
      bool isHovered = el->IsHovered(mouseCoords);

      if(isHovered)
      {
         el->SetState(HoveredState::GetInstance());
         el->SendOnHover();
      }
   }

   void InteractiveElementImpl::LostHoverState::
   OnMousePressed(InteractiveElementImpl* /*el*/, MousePressedEvent& /*e*/)
   {}

   void InteractiveElementImpl::LostHoverState::
   OnMouseReleased(InteractiveElementImpl* /*el*/, MouseReleasedEvent& /*e*/)
   {}

   void InteractiveElementImpl::HoveredState::
   OnMouseMove(InteractiveElementImpl* el, MouseMoveEvent& e)
   {
      math::vec2f mouseCoords(e.GetX(), e.GetY());
      bool isHovered = el->IsHovered(mouseCoords);

      if(!isHovered)
      {
         el->SetState(LostHoverState::GetInstance());
         el->SendOnLostHover();
      }
   }

   void InteractiveElementImpl::HoveredState::
   OnMousePressed(InteractiveElementImpl* el, MousePressedEvent& /*e*/)
   {
      el->SetState(PushedState::GetInstance());
      el->SendOnPush();
   }

   void InteractiveElementImpl::HoveredState::
   OnMouseReleased(InteractiveElementImpl* /*el*/, MouseReleasedEvent& /*e*/)
   {}

   void InteractiveElementImpl::PushedState::
   OnMouseMove(InteractiveElementImpl* el, MouseMoveEvent& e)
   {
      math::vec2f mouseCoords(e.GetX(), e.GetY());
      bool isHovered = el->IsHovered(mouseCoords);

      if(!isHovered)
      {
         el->SetState(LostHoverState::GetInstance());
         el->SendOnLostHover();
      }
   }

   void InteractiveElementImpl::PushedState::
   OnMousePressed(InteractiveElementImpl* /*el*/, MousePressedEvent& /*e*/)
   {}

   void InteractiveElementImpl::PushedState::
   OnMouseReleased(InteractiveElementImpl* el, MouseReleasedEvent& e)
   {
      el->SetState(HoveredState::GetInstance());
      el->SendOnClick(e.GetButton());
      el->SendOnHover();
   }
}
