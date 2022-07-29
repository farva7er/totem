#ifndef _TOTEM_NOVEL_SCRIPT_H_
#define _TOTEM_NOVEL_SCRIPT_H_

namespace totem
{
   class Script
   {
      public:
         virtual void Play() = 0;

         // User Calls, use them inside Play()
         
         void Bg(const char* imagePath);
         
         // End of User Calls
   };
}

#endif
