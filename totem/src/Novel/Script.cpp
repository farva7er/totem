#include "Script.h"
#include "NovelApp.h"

namespace totem
{
   void Script::Bg(const char* imagePath)
   {
      NovelApp::GetInstance()->SetBackground(imagePath);
   }

   void Script::Wait_L()
   {
      NovelApp::GetInstance()->WaitClick();
   }

   int Script::GetOption_L(DialogOptions& dialogOptions)
   {
      return NovelApp::GetInstance()->SetDialogOptions(dialogOptions);
   }

   void Script::ClearDialog()
   {
      NovelApp::GetInstance()->ClearDialogBox();
   }

   bool Script::ScriptShouldYield()
   {
      LOG_INFO("Yield: %b", !NovelApp::GetInstance()->IsScriptPlaying());
      return !NovelApp::GetInstance()->IsScriptPlaying();
   }
}
