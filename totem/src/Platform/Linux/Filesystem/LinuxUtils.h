#ifndef _TOTEM_LINUX_FILESYSTEM_UTILS_H
#define _TOTEM_LINUX_FILESYSTEM_UTILS_H

#include "Filesystem/Utils.h"

namespace totem
{
   class LinuxFileSystemHelper : public FileSystemHelper
   {
   public:
      virtual void SetWorkingDirToExePath() override;
   };
}

#endif
