#ifndef _TOTEM_WINDOWS_FILESYSTEM_UTILS_H
#define _TOTEM_WINDOWS_FILESYSTEM_UTILS_H

#include "Filesystem/Utils.h"

namespace totem
{
   class WindowsFileSystemHelper : public FileSystemHelper
   {
   public:
      virtual void SetWorkingDirToExePath() override;
   };
}

#endif
