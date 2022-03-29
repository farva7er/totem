#ifndef _TOTEM_BASE_ELEMENT_H_
#define _TOTEM_BASE_ELEMENT_H_

#include "IUIElement.h"

namespace totem
{
   class BaseElement : public IUIElement
   {
   public:
      BaseElement()
         :  m_State(State::LostHover),
            m_Pos(math::vec2f(0, 0)),
            m_Scale(math::vec2f(1, 1)),
            m_Listeners(nullptr)
      {}

      virtual ~BaseElement();

      virtual void OnEvent(Event& e) override;
      
      virtual math::vec2f GetPos() const override { return m_Pos; }
      virtual math::vec2f GetScale() const override { return m_Scale; }

      virtual void SetPos(const math::vec2f& pos) override;
      virtual void SetScale(const math::vec2f& scale) override;

      virtual void AddListener(IUIElementListener* listener) override;

   protected:
      virtual bool IsHovered(const math::vec2f& cursorCoords) const = 0;

   private:
      void SendOnLostHover();
      void SendOnHover();
      void SendOnPush();
      void SendOnClick(int button);

   protected:
      enum class State { LostHover, Hovered, Pushed };
      State m_State;

      math::vec2f m_Pos;
      math::vec2f m_Scale;


      struct ListenerNode
      {
         IUIElementListener* m_Listener;
         ListenerNode* m_Next;
         ListenerNode(IUIElementListener* listener,
                     ListenerNode* next = nullptr)
            : m_Listener(listener), m_Next(next)
         {}
         ~ListenerNode() { delete m_Listener; }
      };

      ListenerNode* m_Listeners;
   };
}

#endif

