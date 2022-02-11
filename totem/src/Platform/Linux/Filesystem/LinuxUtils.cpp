#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "LinuxUtils.h"
#include "Assert.h"

namespace totem
{
   void LinuxFileSystemHelper::SetWorkingDirToExePath()
   {
      char buff[250];
      // readlink does not append null byte
      int res = readlink("/proc/self/exe", buff, sizeof(buff) - 1);
      int i = sizeof(buff) - 1;
      while(i >= 0)
      {
         if(buff[i] == '/')
         {
            buff[i] = 0;
            break;
         }
         buff[i] = 0;
         i--;
      }
      TOTEM_ASSERT(res != -1, "/proc/self/exe readlink failed");
      buff[res] = 0;
      res = chdir(buff);
      TOTEM_ASSERT(res != -1, "Chdir for %s failed: %s",
                              buff, strerror(errno));
   }      
}
