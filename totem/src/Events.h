#ifndef _TOTEM_EVENTS_H_
#define _TOTEM_EVENTS_H_

#include <iostream>
#include <string>
#include <sstream>

namespace totem
{

   enum class EventCategory
   {
      Window,
      Keyboard,
      Mouse,
      Other
   };

   enum class EventType
   {
      WindowClose,
      WindowResize,
      FramebufferResize,

      KeyPressed, KeyReleased, KeyRepeat,
      CharPressed,

      MousePressed, MouseReleased,
      MouseMove,
      MouseScroll,

      Other
   };


   class Event
   {
      public:
         Event() : m_IsHandled(false) {}

         static EventType GetStaticType()
         { return EventType::Other; }

         virtual EventCategory GetCategory() const
         { return EventCategory::Other; }

         virtual EventType GetType() const
         { return EventType::Other; }

         bool IsHandled() const { return m_IsHandled; }

         void SetHandled() { m_IsHandled = true; }

         virtual std::string ToString() const = 0;

         template <typename T>
         T& Cast()
         { return *static_cast<T*>(this); }

      private:
         bool m_IsHandled;
   };

   inline std::ostream& operator<< (std::ostream &os, const Event &e)
   {
      return os << e.ToString();
   }

   class IEventListener
   {
      public:
         virtual void OnEvent(Event& e) = 0;
   };

   ////////////////////////////////////////////////////////////
   ////////////// EventDispatcher /////////////////////////////
   //////////////////////////////////////////////////////////// 

   template <typename T>
   class EventDispatcher
   {
      public:
         EventDispatcher(T* obj)
            : m_Obj(obj) {}

         template <typename EventType>
         void Dispatch(void (T::*func)(EventType& e), Event& e)
         {
            if(EventType::GetStaticType() == e.GetType())
               (m_Obj->*func)(*static_cast<EventType*>(&e));
         }

      private:
         T* m_Obj;
   };


   /////////////////////////////////////////////////
   //////// WINDOW EVENTS //////////////////////////
   /////////////////////////////////////////////////
   
   class WindowCloseEvent : public Event
   {
      public:
         static EventType GetStaticType()
         { return EventType::WindowClose; }
         virtual EventCategory GetCategory() const override
         { return EventCategory::Window; }
         virtual EventType GetType() const override
         { return EventType::WindowClose; }
         virtual std::string ToString() const override
         {
            std::stringstream ss;
            ss << "Window Closed.";
            return ss.str();
         }
   };

   class WindowResizeEvent : public Event
   {
      public:
         WindowResizeEvent(int width, int height)
            : m_Width(width), m_Height(height) {}

         static EventType GetStaticType()
         { return EventType::WindowResize; }
         virtual EventCategory GetCategory() const override
         { return EventCategory::Window; }
         virtual EventType GetType() const override
         { return EventType::WindowResize; }
         int GetWidth() const { return m_Width; }
         int GetHeight() const { return m_Height; }
         virtual std::string ToString() const override
         {
            std::stringstream ss;
            ss << "Window Resized: " << "[" << 
               m_Width << ", " << m_Height << "]";
            return ss.str();
         }

      private:
         int m_Width, m_Height;
   };

   class FramebufferResizeEvent : public Event
   {
      public:
         FramebufferResizeEvent(int width, int height)
            : m_Width(width), m_Height(height) {}

         static EventType GetStaticType()
         { return EventType::FramebufferResize; }
         virtual EventCategory GetCategory() const override
         { return EventCategory::Window; }
         virtual EventType GetType() const override
         { return EventType::FramebufferResize; }
         int GetWidth() const { return m_Width; }
         int GetHeight() const { return m_Height; }
         virtual std::string ToString() const override
         {
            std::stringstream ss;
            ss << "Framebuffer Resized: " << "[" << 
               m_Width << ", " << m_Height << "]";
            return ss.str();
         }

      private:
         int m_Width, m_Height;
   };

   //////////////////////////////////////////////////
   /////// KEYBOARD EVENTS //////////////////////////
   //////////////////////////////////////////////////
   

   class KeyEvent : public Event
   {
      public:
         KeyEvent(int code) : m_Code(code) {}

         virtual EventCategory GetCategory() const override
         { return EventCategory::Keyboard; }
         
         int GetCode() const { return m_Code; }
      protected:
         int m_Code;
   };

   class KeyPressedEvent : public KeyEvent
   {
      public:
         KeyPressedEvent(int code)
            : KeyEvent(code) {}

         static EventType GetStaticType()
         { return EventType::KeyPressed; }
         virtual EventType GetType() const override
         { return EventType::KeyPressed; }

         virtual std::string ToString() const override
         {
            std::stringstream ss;
            ss << "Key Pressed: " << m_Code;
            return ss.str();
         }
   };

   class KeyReleasedEvent : public KeyEvent
   {
      public:
         KeyReleasedEvent(int code)
            : KeyEvent(code) {}

         static EventType GetStaticType()
         { return EventType::KeyReleased; }
         virtual EventType GetType() const override
         { return EventType::KeyReleased; }

         virtual std::string ToString() const override
         {
            std::stringstream ss;
            ss << "Key Released: " << m_Code;
            return ss.str();
         }
   };

   class KeyRepeatEvent : public KeyEvent
   {
      public:
         KeyRepeatEvent(int code)
            : KeyEvent(code) {}

         static EventType GetStaticType()
         { return EventType::KeyRepeat; }
         virtual EventType GetType() const override
         { return EventType::KeyRepeat; }

         virtual std::string ToString() const override
         {
            std::stringstream ss;
            ss << "Key Repeat: " << m_Code;
            return ss.str();
         }
   };

   class CharPressedEvent : public KeyEvent
   {
      public:
         CharPressedEvent(int code)
            : KeyEvent(code) {}

         static EventType GetStaticType()
         { return EventType::CharPressed; }
         virtual EventType GetType() const override
         { return EventType::CharPressed; }

         virtual std::string ToString() const override
         {
            std::stringstream ss;
            ss << "Char Pressed: " << m_Code;
            return ss.str();
         }
   };

   /////////////////////////////////////////////////
   ////// MOUSE EVENTS /////////////////////////////
   /////////////////////////////////////////////////
   
   class MouseButtonEvent : public Event
   {
      public:
         MouseButtonEvent(int button) : m_Button(button) {}

         virtual EventCategory GetCategory() const override
         { return EventCategory::Mouse; }
         int GetButton() const { return m_Button; }

      protected:
         int m_Button;
   };

   class MousePressedEvent : public MouseButtonEvent
   {
      public:
         MousePressedEvent(int button)
            : MouseButtonEvent(button) {}

         static EventType GetStaticType()
         { return EventType::MousePressed; }
         virtual EventType GetType() const override
         { return EventType::MousePressed; }

         virtual std::string ToString() const override
         {
            std::stringstream ss;
            ss << "Mouse Button Pressed: " << m_Button;
            return ss.str();
         }
   };

   class MouseReleasedEvent : public MouseButtonEvent
   {
      public:
         MouseReleasedEvent(int button)
            : MouseButtonEvent(button) {}

         static EventType GetStaticType()
         { return EventType::MouseReleased; }
         virtual EventType GetType() const override
         { return EventType::MouseReleased; }

         virtual std::string ToString() const override
         {
            std::stringstream ss;
            ss << "Mouse Button Released: " << m_Button;
            return ss.str();
         }
   };

   class MouseMoveEvent : public Event
   {
      public:
         MouseMoveEvent(float x, float y)
            : m_X(x), m_Y(y) {}

         static EventType GetStaticType()
         { return EventType::MouseMove; }
         virtual EventCategory GetCategory() const override
         { return EventCategory::Mouse; }
         virtual EventType GetType() const override
         { return EventType::MouseMove; }

         float GetX() const { return m_X; }
         float GetY() const { return m_Y; }

         void SetX(float x) { m_X = x; }
         void SetY(float y) { m_Y = y; }

         virtual std::string ToString() const override
         {
            std::stringstream ss;
            ss << "Mouse Moved: " << "[" << m_X << ", " << m_Y << "]";
            return ss.str();
         }

      private:
         float m_X, m_Y;
   };

   class MouseScrollEvent : public Event
   {
      public:
         MouseScrollEvent(double xOffset, double yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset) {}

         static EventType GetStaticType()
         { return EventType::MouseScroll; }
         virtual EventCategory GetCategory() const override
         { return EventCategory::Mouse; }
         virtual EventType GetType() const override
         { return EventType::MouseScroll; }

         virtual std::string ToString() const override
         {
            std::stringstream ss;
            ss << "Mouse Scrolled: " << 
               "[" << m_XOffset << ", " << m_YOffset << "]";
            return ss.str();
         }

      private:
         double m_XOffset, m_YOffset;
   };
}

#endif
