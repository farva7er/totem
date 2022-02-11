#ifndef _TOTEM_FILESYSTEM_UTILS_H_
#define _TOTEM_FILESYSTEM_UTILS_H_

namespace totem
{
   class FileSystemHelper
   {
   public:
      static FileSystemHelper& Get();
      virtual void SetWorkingDirToExePath() = 0;
   protected:
      FileSystemHelper() {}

   private:
      static FileSystemHelper* s_Instance;
   };
}

#endif

