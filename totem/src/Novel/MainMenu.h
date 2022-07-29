#ifndef _TOTEM_NOVEL_MAIN_MENU_H_
#define _TOTEM_NOVEL_MAIN_MENU_H_

#include "UI/Button.h"

namespace totem
{
   class NovelApp;

   class NewGameHandler : public ButtonHandler
   {
      public:
         NewGameHandler(NovelApp* app) : m_App(app) {}
         virtual void OnClick() override;
      private:
         NovelApp* m_App;
   };

   class ExitHandler : public ButtonHandler
   {
      public:
         ExitHandler(NovelApp* app) : m_App(app) {}
         virtual void OnClick() override;
      private:
         NovelApp* m_App;
   };

   class MainMenu
   {
      public:
         MainMenu(NovelApp* app, ResourceManager* resourceManager);
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
