#include "Script.h"

namespace totem
{
   Script::Script()
      : m_App(NovelApp::GetInstance())
   {}

   void Script::Say(const Text& text)
   {
      m_App->SetSpeech(text);
   }

   void Script::Bg(const char* imagePath)
   {
      m_App->SetBackground(imagePath);
   }
}
