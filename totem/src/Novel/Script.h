#ifndef _TOTEM_NOVEL_SCRIPT_H_
#define _TOTEM_NOVEL_SCRIPT_H_

#include "ClientAPI.h"
#include "DialogOptions.h"

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

         // Ask user to select a dialog option.
         // Returns index of selected option.
         int GetOption_L(DialogOptions& dialogOptions);

         // Clear Dialogue Box.
         void ClearDialog();
         
         // End of User Calls.

      protected:
         bool ScriptShouldYield();
   };
}

#endif
