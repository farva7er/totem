#ifndef _TOTEM_WINDOWS_FILESYSTEM_H_
#define _TOTEM_WINDOWS_FILESYSTEM_H_

#include "Filesystem/FileSystem.h"

namespace totem
{
   class WindowsFileSystem : public FileSystem
   {
      public:
         virtual void SetWorkingDirImpl(const char* path) override;
         virtual char* GetExePathImpl() const override;
         virtual char* GetDirImpl(const char* path) const override;
   };
}

#endif
