#include "Filesystem/FileSystem.h"

#if defined(TOTEM_PLATFORM_LINUX)
   #include "Platform/Linux/Filesystem/LinuxFileSystem.h"
#elif defined(TOTEM_PLATFORM_WINDOWS)
   #include "Platform/Windows/Filesystem/WindowsFileSystem.h"
#endif

namespace totem
{
   FileSystem* FileSystem::s_Impl = nullptr;

   void FileSystem::Init()
   {
      if(!s_Impl)
#if defined(TOTEM_PLATFORM_LINUX)
         s_Impl = new LinuxFileSystem();
#elif defined(TOTEM_PLATFORM_WINDOWS)
         s_Impl = new WindowsFileSystem();
#endif
   }

   void FileSystem::SetWorkingDir(const char* path)
   {
      s_Impl->SetWorkingDirImpl(path);
   }

   char* FileSystem::GetExePath()
   {
      return s_Impl->GetExePathImpl();
   }

   char* FileSystem::GetDir(const char* path)
   {
      return s_Impl->GetDirImpl(path);
   }
}
