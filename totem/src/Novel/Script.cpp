#include "Script.h"

namespace totem
{
   void Script::Bg(const char* imagePath)
   {
      NovelApp::GetInstance()->SetBackground(imagePath);
   }
}
