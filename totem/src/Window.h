#ifndef _TOTEM_WINDOW_H_
#define _TOTEM_WINDOW_H_

#include "Events.h"

namespace totem
{
   class Window {
   public:
      Window() {}
      virtual ~Window() = 0;

      static Window* Create(unsigned int Width, unsigned int Height,
                                    const char* Title);
      virtual void OnUpdate() = 0;
      virtual bool IsClosed() = 0;

      virtual void AddEventListener(EventListener *listener) = 0;
      virtual void RemoveEventListener(EventListener *listener) = 0;
      virtual void MakeCurrent() = 0;
      virtual void* GetOpenGLLoaderFunc() = 0;
      virtual unsigned int GetFBWidth() = 0;
      virtual unsigned int GetFBHeight() = 0;

   private:
      unsigned int m_Width, m_Height;
      const char *m_Title;
   };
}

#endif
