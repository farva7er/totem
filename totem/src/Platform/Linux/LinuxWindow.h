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
      virtual bool IsClosed() const override;

      void OnEvent(Event& e) const;
      virtual void AddEventListener(IEventListener* listener,
                                    int priority) override;
      virtual void RemoveEventListener(IEventListener* listener) override;
      virtual void MakeCurrent() override;
      virtual void* GetOpenGLLoaderFunc() const override;
      virtual void SendInitEvents() const override;
      virtual unsigned int GetFBWidth() const override;
      virtual unsigned int GetFBHeight() const override;
      virtual unsigned int GetWidth() const override;
      virtual unsigned int GetHeight() const override;
      virtual float GetContentScaleX() const override;
      virtual float GetContentScaleY() const override;

   private:
      void CleanEventListeners();
   private:
      static GLFWwindow* s_CurrentWindow;
   private:
      struct EventListenerNode
      {
         IEventListener* listener;
         int priority;
         EventListenerNode* next;

         EventListenerNode(IEventListener* l, int p,
                           EventListenerNode* n = nullptr)
         {
            listener = l;
            priority = p;
            next = n;
         }
      };

      GLFWwindow* m_glfwWindow;
      EventListenerNode* m_EventListenerListHead; 
   };
}

#endif
