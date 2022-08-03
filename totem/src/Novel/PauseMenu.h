#ifndef _TOTEM_NOVEL_PAUSE_MENU_H_
#define _TOTEM_NOVEL_PAUSE_MENU_H_

#include "UI/Button.h"

namespace totem
{
   class PauseHandler : public ButtonHandler
   {
      public:
         virtual void OnClick() override;
   };

   class UnPauseHandler : public ButtonHandler
   {
      public:
         virtual void OnClick() override;
   }; 

   class MainMenuHandler : public ButtonHandler
   {
      public:
         virtual void OnClick() override;
   }; 

   class PauseMenu
   {
      public: 
         PauseMenu();
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
