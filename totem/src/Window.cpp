#include "Window.h"
#include "Log.h"

#if defined(TOTEM_PLATFORM_LINUX)
   #include "Platform/Linux/LinuxWindow.h"
#elif defined(TOTEM_PLATFORM_WINDOWS)
   // Temporary
   #include "Platform/Linux/LinuxWindow.h"
#endif

namespace totem
{
   Window* Window::Create( unsigned int Width, unsigned int Height,
                                 const char* Title)
   {
#if defined(TOTEM_PLATFORM_LINUX)
      return new LinuxWindow(Width, Height, Title);
#elif defined(TOTEM_PLATFORM_WINDOWS)
      // Temporary
      return new LinuxWindow(Width, Height, Title);
#else
      TOTEM_ASSERT(false, "This Platform is not supported.");
      return nullptr;
#endif
   }

   Window::~Window() {}
}
