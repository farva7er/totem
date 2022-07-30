#ifndef _TOTEM_NOVEL_PAUSE_MENU_H_
#define _TOTEM_NOVEL_PAUSE_MENU_H_

#include "UI/Button.h"

namespace totem
{
   class NovelApp;

   class PauseHandler : public ButtonHandler
   {
      public:
         PauseHandler(NovelApp* app);

         virtual void OnClick() override;

      private:
         NovelApp* m_App;
   };

   class UnPauseHandler : public ButtonHandler
   {
      public:
         UnPauseHandler(NovelApp* app);

         virtual void OnClick() override;

      private:
         NovelApp* m_App;
   }; 

   class MainMenuHandler : public ButtonHandler
   {
      public:
         MainMenuHandler(NovelApp* app);

         virtual void OnClick() override;

      private:
         NovelApp* m_App;
   }; 

   class PauseMenu
   {
      public: 
         PauseMenu(NovelApp* app, ResourceManager* resourceManager);
         ~PauseMenu();

         void SetActive(bool isActive);

         void OnEvent(Event& e);
         void OnUpdate(float deltaTime);
         void Draw(Renderer* renderer);

      private:
         bool m_IsActive;

         Button* m_PauseButton;
         ButtonHandler* m_PauseHandler;
         
         Button* m_MainMenuButton;
         ButtonHandler* m_MainMenuHandler;

         Button* m_UnPauseButton;
         ButtonHandler* m_UnPauseHandler;
   };
}

#endif
