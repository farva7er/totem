#ifndef _TOTEM_NOVEL_SCRIPT_H_
#define _TOTEM_NOVEL_SCRIPT_H_

#include "NovelApp.h"
#include "Internationalization/Text.h"

namespace totem
{
   class Script
   {
      public:
         Script();
         virtual void Play() = 0;

         // User Calls, use them inside Play()

         void Say(const Text& text);
         void Bg(const char* imagePath);
         
         // End of User Calls
      private:
         NovelApp* m_App;
   };
}

#endif
