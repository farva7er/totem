#include <GLFW/glfw3.h>

#include "LinuxWindow.h"
#include "Log.h"

namespace totem
{
   GLFWwindow* LinuxWindow::s_CurrentWindow = nullptr;

   static void window_close_callback(GLFWwindow* window)
   {
      WindowCloseEvent e;
      LinuxWindow* w = (LinuxWindow*)glfwGetWindowUserPointer(window);
      w->OnEvent(e);
   }

   static void window_size_callback(GLFWwindow* window, int width, int height)
   {
      WindowResizeEvent e(width, height);

      LinuxWindow* w = (LinuxWindow*)glfwGetWindowUserPointer(window);
      w->OnEvent(e);
   }

   static void framebuffer_size_callback(GLFWwindow* window, 
                                          int width, int height)
   {
      FramebufferResizeEvent e(width, height);

      LinuxWindow* w = (LinuxWindow*)glfwGetWindowUserPointer(window);
      w->OnEvent(e); 
   }

   static void key_callback(GLFWwindow* window,
                           int key, int scancode, int action, int mods)
   {
      LinuxWindow* w = (LinuxWindow*)glfwGetWindowUserPointer(window);

      if(action == GLFW_PRESS)
      {
         KeyPressedEvent e(key);
         w->OnEvent(e);

      } else if (action == GLFW_REPEAT)
      {
         KeyRepeatEvent e(key);
         w->OnEvent(e);

      } else if (action == GLFW_RELEASE)
      {
         KeyReleasedEvent e(key);
         w->OnEvent(e);
      }
   }

   static void character_callback(GLFWwindow* window,
                                 unsigned int codepoint)
   {
      CharPressedEvent e(codepoint);
      LinuxWindow* w = (LinuxWindow*)glfwGetWindowUserPointer(window);
      w->OnEvent(e);
   }

   static void cursor_position_callback(GLFWwindow* window, 
                                       double xpos, double ypos)
   {
      MouseMoveEvent e((int)xpos, (int)ypos);
      LinuxWindow* w = (LinuxWindow*)glfwGetWindowUserPointer(window);
      w->OnEvent(e);
   }

   void mouse_button_callback(GLFWwindow* window,
                              int button, int action, int mods)
   {
      LinuxWindow* w = (LinuxWindow*)glfwGetWindowUserPointer(window);
      if (action == GLFW_PRESS)
      {
         if(button == GLFW_MOUSE_BUTTON_LEFT)
         {
            MousePressedEvent e(0);
            w->OnEvent(e);
         } else if (button == GLFW_MOUSE_BUTTON_RIGHT)
         {
            MousePressedEvent e(1);
            w->OnEvent(e);
         }
      } else if (action == GLFW_RELEASE) {
         if(button == GLFW_MOUSE_BUTTON_LEFT)
         {
            MouseReleasedEvent e(0);
            w->OnEvent(e);
         } else if (button == GLFW_MOUSE_BUTTON_RIGHT)
         {
            MouseReleasedEvent e(1);
            w->OnEvent(e);
         }
      }  
   }

   void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
   {
      MouseScrollEvent e(xoffset, yoffset);
      LinuxWindow* w = (LinuxWindow*)glfwGetWindowUserPointer(window);
      w->OnEvent(e);
   }

   LinuxWindow::LinuxWindow(unsigned int Width, unsigned int Height,
                              const char* Title)
      : m_EventListenerListHead(nullptr)
   {

      /* Initialize the library */
      if (!glfwInit())
      {
         LOG_FATAL("GLFW Failed to initialize!");
         return;
      }

      /* Create a windowed mode window and its OpenGL context */
      m_glfwWindow = glfwCreateWindow(Width, Height, Title, NULL, NULL);
      if (!m_glfwWindow)
      {
         LOG_FATAL("GLFW Failed to create a window!");
         glfwTerminate();
         return;
      }


      glfwSetWindowUserPointer(m_glfwWindow, this);
  
      glfwSetWindowCloseCallback(m_glfwWindow, window_close_callback);
      glfwSetWindowSizeCallback(m_glfwWindow, window_size_callback);
      glfwSetFramebufferSizeCallback(m_glfwWindow, framebuffer_size_callback);
      glfwSetKeyCallback(m_glfwWindow, key_callback);
      glfwSetCharCallback(m_glfwWindow, character_callback);
      glfwSetCursorPosCallback(m_glfwWindow, cursor_position_callback);
      glfwSetMouseButtonCallback(m_glfwWindow, mouse_button_callback);
      glfwSetScrollCallback(m_glfwWindow, scroll_callback);

      glfwSwapInterval(1);
   }

   LinuxWindow::~LinuxWindow()
   {
      CleanEventListeners();
      glfwTerminate();
   }

   void LinuxWindow::OnUpdate()
   {
      glfwPollEvents();
      glfwSwapBuffers(m_glfwWindow);
   }

   bool LinuxWindow::IsClosed() const
   {
      return glfwWindowShouldClose(m_glfwWindow);
   }

   void LinuxWindow::OnEvent(Event& e) const
   {
      EventListenerNode* curr = m_EventListenerListHead;
      while(curr)
      {
         curr->listener->OnEvent(e);
         curr = curr->next;
      }
   }


   void LinuxWindow::AddEventListener(IEventListener* listener)
   {

      EventListenerNode *newNode = 
         new EventListenerNode(listener, m_EventListenerListHead);
      m_EventListenerListHead = newNode;
   }

   void LinuxWindow::RemoveEventListener(IEventListener* listener)
   {
      for(EventListenerNode** curr = &m_EventListenerListHead; 
            *curr;
            curr = &((*curr)->next))
      {
         if((*curr)->listener == listener)
         {
            EventListenerNode* tmp = *curr;
            *curr = (*curr)->next;
            delete tmp;
            break;
         }
      }
   }

   void LinuxWindow::CleanEventListeners()
   {

      while(m_EventListenerListHead)
      {
         EventListenerNode* tmp = m_EventListenerListHead->next;
         delete m_EventListenerListHead;
         m_EventListenerListHead = tmp;
      }
   }

   void LinuxWindow::MakeCurrent()
   {
      if(s_CurrentWindow != m_glfwWindow)
      { 
         glfwMakeContextCurrent(m_glfwWindow);
         s_CurrentWindow = m_glfwWindow;
      }
   }

   void* LinuxWindow::GetOpenGLLoaderFunc() const
   {
      return reinterpret_cast<void*>(glfwGetProcAddress);
   }

   void LinuxWindow::SendInitEvents() const
   {
      FramebufferResizeEvent fe(GetFBWidth(), GetFBHeight());
      OnEvent(fe);
      WindowResizeEvent we(GetWidth(), GetHeight());
      OnEvent(we);
   }

   unsigned int LinuxWindow::GetFBWidth() const
   {
      int width, height;
      glfwGetFramebufferSize(m_glfwWindow, &width, &height);
      return width;
   }

   unsigned int LinuxWindow::GetFBHeight() const
   {
      int width, height;
      glfwGetFramebufferSize(m_glfwWindow, &width, &height);
      return height;
   }

   unsigned int LinuxWindow::GetWidth() const
   {
      int width, height;
      glfwGetWindowSize(m_glfwWindow, &width, &height);
      return width;
   }

   unsigned int LinuxWindow::GetHeight() const
   {
      int width, height;
      glfwGetWindowSize(m_glfwWindow, &width, &height);
      return height;
   }

   float LinuxWindow::GetContentScaleX() const
   {
      float xscale, yscale;
      glfwGetWindowContentScale(m_glfwWindow, &xscale, &yscale);
      return xscale;
   }

   float LinuxWindow::GetContentScaleY() const
   {
      float xscale, yscale;
      glfwGetWindowContentScale(m_glfwWindow, &xscale, &yscale);
      return yscale;
   }
}

