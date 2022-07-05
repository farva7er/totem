#include "Script.h"

namespace totem
{
   void Script::Say(const Text& text)
   {
      NovelApp::GetInstance()->SetSpeech(text);
   }

   void Script::Bg(const char* imagePath)
   {
      NovelApp::GetInstance()->SetBackground(imagePath);
   }
}
