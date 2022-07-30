#include "PauseMenu.h"
#include "NovelApp.h"

namespace totem
{
   PauseHandler::PauseHandler(NovelApp* app)
      : m_App(app)
   {}

   void PauseHandler::OnClick()
   {
      m_App->Pause();
   }

   UnPauseHandler::UnPauseHandler(NovelApp* app)
      : m_App(app)
   {}

   void UnPauseHandler::OnClick()
   {
      m_App->UnPause();
   }

   MainMenuHandler::MainMenuHandler(NovelApp* app)
      : m_App(app)
   {}

   void MainMenuHandler::OnClick()
   {
      m_App->ExitToMainMenu();
   }

   PauseMenu::PauseMenu(NovelApp* app, ResourceManager* resourceManager)
   {
      Ref<Font> defaultFont = 
         resourceManager->Get<Font>("resources/fonts/OpenSans-Regular.ttf");

      m_PauseButton = new Button(defaultFont);
      m_PauseButton->SetPos({ 14, 7 });
      m_PauseButton->SetScale({ 1, 1 });
      m_PauseButton->SetText("II");
      m_PauseButton->SetColor({ 0, 0, 0, 1 });

      m_PauseHandler = new PauseHandler(app);
      m_PauseButton->SetHandler(m_PauseHandler);

      m_MainMenuButton = new Button(defaultFont);
      m_MainMenuButton->SetPos({ 0, 0 });
      m_MainMenuButton->SetScale({ 2.5f, 0.7f });
      m_MainMenuButton->SetText("Exit to Main Menu");

      m_MainMenuHandler = new MainMenuHandler(app);
      m_MainMenuButton->SetHandler(m_MainMenuHandler);

      m_UnPauseButton = new Button(defaultFont);
      m_UnPauseButton->SetPos({ 3.3f, 1.3f });
      m_UnPauseButton->SetScale({ 0.3f, 0.3f });
      m_UnPauseButton->SetText("X");
      m_UnPauseButton->SetColor({ 0.6f, 0.2f, 0.2f, 0.8f });

      m_UnPauseHandler = new UnPauseHandler(app);
      m_UnPauseButton->SetHandler(m_UnPauseHandler);
   }

   PauseMenu::~PauseMenu()
   {
      delete m_PauseButton;
      delete m_PauseHandler;

      delete m_MainMenuButton;
      delete m_MainMenuHandler;

      delete m_UnPauseButton;
      delete m_UnPauseHandler;
   }

   void PauseMenu::SetActive(bool isActive)
   {
      m_IsActive = isActive;
      m_MainMenuButton->SetActive(isActive);
      m_UnPauseButton->SetActive(isActive);
      m_PauseButton->SetActive(!isActive); 
   }

   void PauseMenu::OnEvent(Event& e)
   {
      m_MainMenuButton->OnEvent(e);
      m_UnPauseButton->OnEvent(e);
      m_PauseButton->OnEvent(e);
   }

   void PauseMenu::OnUpdate(float deltaTime)
   {
      m_MainMenuButton->OnUpdate(deltaTime);
      m_UnPauseButton->OnUpdate(deltaTime);
      m_PauseButton->OnUpdate(deltaTime);
   }

   void PauseMenu::Draw(Renderer* renderer)
   {
      if(m_IsActive)
      {
         Rect rect;
         rect
            .SetPos({ 0, 0 })
            .SetScale({ 4, 2 })
            .SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });
         renderer->DrawRect(rect);
         m_MainMenuButton->Draw(renderer);
         m_UnPauseButton->Draw(renderer);
      }
      else
      {
         m_PauseButton->Draw(renderer);
      }
   }
}
