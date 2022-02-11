#include "Filesystem/Utils.h"

#if defined(TOTEM_PLATFORM_LINUX)
   #include "Platform/Linux/Filesystem/LinuxUtils.h"
#elif defined(TOTEM_PLATFORM_WINDOWS)
   #include "Platform/Windows/Filesystem/WindowsUtils.h"
#endif

namespace totem
{
   FileSystemHelper* FileSystemHelper::s_Instance = nullptr;

   FileSystemHelper& FileSystemHelper::Get()
   {
      if(!s_Instance)
#if defined(TOTEM_PLATFORM_LINUX)
         s_Instance = new LinuxFileSystemHelper();
#elif defined(TOTEM_PLATFORM_WINDOWS)
         s_Instance = new WindowsFileSystemHelper();
#endif
      return *s_Instance;
   }
}
