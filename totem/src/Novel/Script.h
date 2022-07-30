#ifndef _TOTEM_NOVEL_SCRIPT_H_
#define _TOTEM_NOVEL_SCRIPT_H_

#include "ClientAPI.h"

namespace totem
{
   class Script
   {
      public:
         virtual void Play() = 0;

         // User Calls, use them inside Play().
         
         void Bg(const char* imagePath);

         // Wait for player to click.
         void Wait_L();

         // Clear Dialogue Box.
         void ClearDialog();
         
         // End of User Calls.

      protected:
         bool ScriptShouldYield();
   };
}

#endif
