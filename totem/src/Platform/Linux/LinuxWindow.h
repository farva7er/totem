#ifndef _TOTEM_LINUX_WINDOW_H_
#define _TOTEM_LINUX_WINDOW_H_

#include <GLFW/glfw3.h>

#include "Window.h"
#include "Events.h"

namespace totem
{
   class LinuxWindow : public Window
   {
   public:
      LinuxWindow(unsigned int Width, unsigned int Height,
                  const char* Title);
      virtual ~LinuxWindow() override;

      virtual void OnUpdate() override;
      virtual bool IsClosed() override;

      void OnEvent(Event& e) const;
      virtual void AddEventListener(EventListener* listener) override;
      virtual void RemoveEventListener(EventListener* listener) override;
      virtual void MakeCurrent() override;
      virtual void* GetOpenGLLoaderFunc() override;
   private:
      void CleanEventListeners();
   private:
      static GLFWwindow* s_CurrentWindow;
   private:
      struct EventListenerNode
      {
         EventListener* listener;
         EventListenerNode* next;

         EventListenerNode(EventListener* l, EventListenerNode* n = nullptr)
         {
            listener = l;
            next = n; 
         }
      };

      GLFWwindow* m_glfwWindow;
      EventListenerNode* m_EventListenerListHead; 
   };
}

#endif