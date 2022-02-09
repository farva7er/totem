#ifndef _TOTEM_ASSERT_H_
#define _TOTEM_ASSERT_H_

#include "Log.h"

#if defined(TOTEM_DEBUG)
   #if defined(TOTEM_PLATFORM_LINUX)
      #include <signal.h>
      #define TOTEM_DEBUGBREAK() raise(SIGTRAP)
   #elif defined(TOTEM_PLATFORM_WINDOWS)
      #define TOTEM_DEBUGBREAK() __debugbreak()
   #endif

   #define TOTEM_ASSERT(cond, ...)                             \
      {                                                        \
         if(!(cond)) {                                         \
            LOG_FATAL("%s:%s - Assertion failed: %s",          \
                        __FILE__, TOSTRING(__LINE__), #cond);  \
            LOG_FATAL(__VA_ARGS__);                            \
            TOTEM_DEBUGBREAK();                                \
         }                                                     \
      }

#else
   #define TOTEM_ASSERT(cond, ...)
#endif

#endif
