#ifndef _TOTEM_INTERACTIVE_ELEMENT_IMPL_H_
#define _TOTEM_INTERACTIVE_ELEMENT_IMPL_H_

#include "BasicElementImpl.h"
#include "IIEListener.h"

namespace totem
{
   class InteractiveElementImpl : public BasicElementImpl
   {
      class State;
   public:
      InteractiveElementImpl()
         :  m_IsActive(true),
            m_State(LostHoverState::GetInstance()),
            m_Listeners(nullptr)
      {}

      virtual ~InteractiveElementImpl();
      InteractiveElementImpl(const InteractiveElementImpl& other);
      InteractiveElementImpl& operator=(const InteractiveElementImpl& other);
   
      bool IsActive() const
      { return m_IsActive; }
      void SetActive(bool isActive);

      virtual void OnEvent(Event& e);
      void AddListener(IIEListener* listener);
      void RemoveListener(IIEListener* listener);
      void ClearListeners();

   protected:
      virtual bool IsHovered(const math::vec2f& cursorCoords) const;
      virtual void OnLostHover() {};
      virtual void OnHover() {};
      virtual void OnPush() {};
      virtual void OnClick(int /*button*/) {};

   private:
      void SendOnLostHover();
      void SendOnHover();
      void SendOnPush();
      void SendOnClick(int button);

      void OnMouseMove(MouseMoveEvent& e);
      void OnMousePressed(MousePressedEvent& e);
      void OnMouseReleased(MouseReleasedEvent& e);
      void SetState(State* newState);
   private:
      class State
      {
      public:
         virtual void OnMouseMove(InteractiveElementImpl* el,
                                 MouseMoveEvent& e) = 0;
         virtual void OnMousePressed(InteractiveElementImpl* el,
                                 MousePressedEvent& e) = 0;
         virtual void OnMouseReleased(InteractiveElementImpl* el,
                                 MouseReleasedEvent& e) = 0;
      protected:
         void ChangeState(InteractiveElementImpl* el, State* newState);
      };

      class LostHoverState : public State
      {
      public:
         static LostHoverState* GetInstance()
         { return &s_Instance; }

         virtual void OnMouseMove(InteractiveElementImpl* el,
                                 MouseMoveEvent& e) override;
         virtual void OnMousePressed(InteractiveElementImpl* el,
                                 MousePressedEvent& e) override;
         virtual void OnMouseReleased(InteractiveElementImpl* el,
                                 MouseReleasedEvent& e) override;
      private:
         LostHoverState() {}
         static LostHoverState s_Instance;
      };

      class HoveredState : public State
      {
      public:
         static HoveredState* GetInstance()
         { return &s_Instance; }
         virtual void OnMouseMove(InteractiveElementImpl* el,
                                 MouseMoveEvent& e) override;
         virtual void OnMousePressed(InteractiveElementImpl* el,
                                 MousePressedEvent& e) override;
         virtual void OnMouseReleased(InteractiveElementImpl* el,
                                 MouseReleasedEvent& e) override;
      private:
         HoveredState() {}
         static HoveredState s_Instance;
      };

      class PushedState : public State
      {
      public:
         static PushedState* GetInstance()
         { return &s_Instance; }
         virtual void OnMouseMove(InteractiveElementImpl* el,
                                 MouseMoveEvent& e) override;
         virtual void OnMousePressed(InteractiveElementImpl* el,
                                 MousePressedEvent& e) override;
         virtual void OnMouseReleased(InteractiveElementImpl* el,
                                 MouseReleasedEvent& e) override;
      private:
         PushedState() {}
         static PushedState s_Instance;
      };

      struct ListenerNode
      {
         IIEListener* m_Listener;
         ListenerNode* m_Next;
         ListenerNode(IIEListener* listener,
                     ListenerNode* next = nullptr)
            : m_Listener(listener), m_Next(next)
         {}
         ~ListenerNode() { delete m_Listener; }
      };

      bool m_IsActive;
      State* m_State;
      ListenerNode* m_Listeners; 
   };
}

#endif
