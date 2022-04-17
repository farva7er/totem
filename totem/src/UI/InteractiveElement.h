#include "MovableElement.h"

namespace totem
{
   class IIEListener
   {
   public:
      IIEListener() = default;
      virtual ~IIEListener() = default;
      IIEListener(const IIEListener& other) = default;
      IIEListener& operator=(const IIEListener& other) = default;

      virtual IIEListener* Clone() const = 0;

      virtual void OnClick(int btn) {}
      virtual void OnLostHover() {}
      virtual void OnHover() {}
      virtual void OnPush() {}
   };


   class InteractiveElement : public MovableElement
   {
   public:
      InteractiveElement()
         :  m_State(State::LostHover),
            m_Listeners(nullptr)
      {}
      virtual ~InteractiveElement();
      InteractiveElement(const InteractiveElement& other);
      InteractiveElement& operator=(const InteractiveElement& other);
            
      virtual void OnEvent(Event& e) override;
      void AddListener(IIEListener* listener);
      void ClearListeners();

   protected:
      virtual bool IsHovered(const math::vec2f& cursorCoords) const;

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
      State m_State;
      ListenerNode* m_Listeners; 
   };
}
