#include <Windows.h>
#include <direct.h>
#include <string.h>
#include "WindowsFileSystem.h"
#include "Assert.h"

namespace totem
{
   void WindowsFileSystem::SetWorkingDirImpl(const char* path)
   {
      int res = chdir(path);
      if(-1 == res)
         LOG_ERROR("Couldn't set working directory (chdir failed) %s",
                  strerror(errno));
   }

   char* WindowsFileSystem::GetExePathImpl() const
   {
      enum { buffLen = 250; }
      char* buff = new char[buffLen];

      // WinXP does not append null byte
      DWORD len = GetModuleFileNameA(NULL, buff, buffLen - 1);
      if(len != -1)
      {
         buff[len] = 0;
         return buff;
      }
      else
      {
         LOG_ERROR("Couldn't get exe path (GetModuleFileNameA failed) %s",
                  strerror(errno));
         return nullptr;
      }
   }

   char* WindowsFileSystem::GetDirImpl(const char* path) const
   {
      if(!path)
         return nullptr;

      int len = strlen(path);
      if(len <= 0)
      {
         char* res = new char[1];
         *res = 0;
         return res;
      }

      char* res = new char[len + 1];
      strncpy(res, path, len);
      res[len] = 0;

      int i = len - 1;
      while(i >= 0)
      {
         if(res[i] == '\\')
         {
            res[i] = 0;
            break;
         }
         i--;
      }
      return res;
   }
}
