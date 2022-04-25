#ifndef _TOTEM_INTERACTIVE_ELEMENT_IMPL_H_
#define _TOTEM_INTERACTIVE_ELEMENT_IMPL_H_

#include "BasicElementImpl.h"
#include "IIEListener.h"

namespace totem
{
   class InteractiveElementImpl : public BasicElementImpl
   {
   public:
      InteractiveElementImpl()
         :  m_IsActive(true),
            m_State(State::LostHover),
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

   private:
      enum class State { LostHover, Hovered, Pushed };
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
      State m_State;
      ListenerNode* m_Listeners; 
   };
}

#endif
