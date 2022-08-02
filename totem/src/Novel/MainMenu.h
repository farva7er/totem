#ifndef _TOTEM_NOVEL_MAIN_MENU_H_
#define _TOTEM_NOVEL_MAIN_MENU_H_

#include "UI/Button.h"

namespace totem
{
   class NovelApp;

   class NewGameHandler : public ButtonHandler
   {
      public:
         virtual void OnClick() override;
   };

   class ExitHandler : public ButtonHandler
   {
      public:
         virtual void OnClick() override;
   };

   class MainMenu
   {
      public:
         MainMenu();
         ~MainMenu();

         void OnEvent(Event& e);
         void OnUpdate(float deltaTime);
         void Draw(Renderer* renderer);

      private:

         Button* m_NewGameButton;
         ButtonHandler* m_NewGameHandler;

         Button* m_ExitButton;
         ButtonHandler* m_ExitHandler;
   };
}

#endif
