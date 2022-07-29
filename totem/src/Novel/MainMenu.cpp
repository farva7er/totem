#include "MainMenu.h"
#include "NovelApp.h"

namespace totem
{
   void NewGameHandler::OnClick()
   {
      m_App->PlayScript(0);
   }

   void ExitHandler::OnClick()
   {
      m_App->OnExit();
   }

   MainMenu::MainMenu(NovelApp* app, ResourceManager* resourceManager)
   {
      Ref<Font> defaultFont = 
         resourceManager->Get<Font>("resources/fonts/OpenSans-Regular.ttf");

      m_NewGameButton = new Button(defaultFont);
      m_NewGameButton->SetPos({0, 2});
      m_NewGameButton->SetScale({4, 1.5f});
      m_NewGameButton->SetText("New Game");
      m_NewGameHandler = new NewGameHandler(app);
      m_NewGameButton->SetHandler(m_NewGameHandler);

      m_ExitButton = new Button(defaultFont);
      m_ExitButton->SetPos({0, -2});
      m_ExitButton->SetScale({4, 1.5f});
      m_ExitButton->SetText("Exit");
      m_ExitHandler = new ExitHandler(app);
      m_ExitButton->SetHandler(m_ExitHandler);
   }

   MainMenu::~MainMenu()
   {
      delete m_NewGameButton;
      delete m_NewGameHandler;

      delete m_ExitButton;
      delete m_ExitHandler;
   }

   void MainMenu::OnEvent(Event& e)
   {
      m_NewGameButton->OnEvent(e);
      m_ExitButton->OnEvent(e);
   }

   void MainMenu::OnUpdate(float deltaTime)
   {
      m_NewGameButton->OnUpdate(deltaTime);
      m_ExitButton->OnUpdate(deltaTime);
   }

   void MainMenu::Draw(Renderer* renderer)
   {
      renderer->Clear({0.1f, 0.4f, 0.5f, 1.0f});
      m_NewGameButton->Draw(renderer);
      m_ExitButton->Draw(renderer);
   }
}
