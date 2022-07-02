#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "LinuxFileSystem.h"
#include "Assert.h"

namespace totem
{
   void LinuxFileSystem::SetWorkingDirImpl(const char* path)
   {
      int res = chdir(path);
      if(-1 == res)
         LOG_ERROR("Couldn't set working directory (chdir failed) %s",
                  strerror(errno));
   }

   char* LinuxFileSystem::GetExePathImpl() const
   {
      enum { buffLen = 250 };
      char* buff = new char[buffLen];
      // readlink does not append null byte
      int len = readlink("/proc/self/exe", buff, buffLen - 1);
      if(len != -1)
      {
         buff[len] = 0;
         return buff;
      }
      else
      {
         LOG_ERROR("Couldn't get exe path (readlink failed) %s",
                  strerror(errno));
         return nullptr;
      }
   }

   char* LinuxFileSystem::GetDirImpl(const char* path) const
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
         if(res[i] == '/')
         {
            res[i] = 0;
            break;
         }
         i--;
      }
      return res;
   }    
}
