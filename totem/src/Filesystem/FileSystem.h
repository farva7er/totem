#ifndef _TOTEM_FILESYSTEM_H_
#define _TOTEM_FILESYSTEM_H_

namespace totem
{
   class FileSystem
   {
      public:
         static void Init();

         static void SetWorkingDir(const char* path);
         static char* GetExePath();
         static char* GetDir(const char* path);

      private:
         virtual void SetWorkingDirImpl(const char* path) = 0;
         virtual char* GetExePathImpl() const = 0;
         virtual char* GetDirImpl(const char* path) const = 0;

      private:
         static FileSystem* s_Impl;
   };
}

#endif

