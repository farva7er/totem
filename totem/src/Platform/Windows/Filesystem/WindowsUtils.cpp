#include <Windows.h>
#include <direct.h>
#include <string.h>
#include "WindowsUtils.h"
#include "Assert.h"

namespace totem
{
   void WindowsFileSystemHelper::SetWorkingDirToExePath()
   {
      char buff[250];
      // WinXP does not append null byte
      DWORD res = GetModuleFileNameA(NULL, buff, sizeof(buff) - 1);
      int i = res - 1;
      while(i >= 0)
      {
         if(buff[i] == '\\')
         {
            buff[i] = 0;
            break;
         }
         buff[i] = 0;
         i--;
      }
      TOTEM_ASSERT(res != -1, "/proc/self/exe readlink failed");
      res = chdir(buff);
      TOTEM_ASSERT(res != -1, "Chdir for %s failed: %s",
                              buff, strerror(errno));
   }
}