#include "NovelApp.h"
#include "Window.h"
#include "Rendering/Renderer.h"
#include "Timer.h"
#include "Core/ResourceManager.h"
#include "Filesystem/FileSystem.h"
#include "Handlers/SpeechHandler.h"
#include "Handlers/ExitLoopHandler.h"
#include "Handlers/WaitClickHandler.h"
#include "Handlers/IdleHandler.h"
#include "MainMenu.h"
#include "PauseMenu.h"

#include <stdlib.h>

namespace totem
{
   NovelApp* NovelApp::s_Instance = nullptr;

   NovelApp* NovelApp::GetInstance()
   {
      TOTEM_ASSERT(s_Instance, "Novel App is not created!!!")
      return s_Instance;
   }

   ResourceManager* NovelApp::GetResourceManager()
   {
      TOTEM_ASSERT(s_Instance, "Novel App is not created!!!");
      return s_Instance->m_ResourceManager;
   }

   NovelApp::NovelApp()
      : App(0, nullptr)
   {
      m_State = State::MainMenu;
      m_IsPaused = false;

      s_Instance = this;

      FileSystem::Init();

      char* exePath = FileSystem::GetExePath();
      char* exeDir = FileSystem::GetDir(exePath);
      FileSystem::SetWorkingDir(exeDir);
      delete [] exePath;
      delete [] exeDir;

      m_Window = Window::Create(1280, 720, "Totem");
      m_Window->AddEventListener(this);

      m_ResourceManager = new ResourceManager();

      m_Renderer = new Renderer(m_Window,
         m_ResourceManager->
            Get<Shader>("resources/shaders/DefTexture.glsl", false),
         m_ResourceManager->
            Get<Shader>("resources/shaders/DefFont.glsl", false));
 
      m_CharacterScene = new CharacterScene();

      m_CurrentHandler = nullptr;

      m_DialogBox = new totem::DialogBox();

      m_Background = nullptr;

      m_ScriptRegistry = new ScriptRegistry();

      m_MainMenu = new MainMenu();
      m_PauseMenu = new PauseMenu();

      SetCanvasScale({ 16, 9 });

      m_Window->SendInitEvents();
   }

   NovelApp::~NovelApp()
   {
      OnExit();
   }

   void NovelApp::SetHandler(NovelHandler* handler)
   {
      if(m_CurrentHandler)
         delete m_CurrentHandler;
      m_CurrentHandler = handler;
   }

   void NovelApp::AddScript(Script& script)
   {
      m_ScriptRegistry->AddScript(&script);
   }

   void NovelApp::PlayScript(int scriptIndex)
   {
      m_State = State::PlayingScript;
      m_IsPaused = false;
      m_PauseMenu->SetActive(false);
      m_CharacterScene->Clear();
      m_DialogBox->ClearDialogOptions();
      m_DialogBox->SetText("");

      (*m_ScriptRegistry)[scriptIndex].Play();

      m_State = State::MainMenu;
      m_Window->SendInitEvents();
   }

   void NovelApp::StartMainMenu()
   {
      m_State = State::MainMenu;
      m_Window->SendInitEvents();
      Loop();
   }

   void NovelApp::ExitToMainMenu()
   {
      m_State = State::MainMenu;
      m_Window->SendInitEvents();
      SetHandler(new ExitLoopHandler());
   }

   bool NovelApp::IsScriptPlaying() const
   {
      return m_State == State::PlayingScript;
   }

   void NovelApp::Pause()
   {
      m_IsPaused = true;
      m_PauseMenu->SetActive(true);
   }

   void NovelApp::UnPause()
   {
      m_IsPaused = false;
      m_PauseMenu->SetActive(false);
   }

   void NovelApp::ClearDialogBox()
   {
      m_DialogBox->SetText("");
      m_DialogBox->SetCharacterName("");
   }

   void NovelApp::WaitClick()
   {
      SetHandler(new WaitClickHandler());
      Loop();
   }

   void NovelApp::ShowCharacter(const Character& character, int slot)
   {
      m_CharacterScene->Add(&character, slot);
      WaitClick();
   }

   void NovelApp::HideCharacter(const Character& character)
   {
      m_CharacterScene->Remove(&character);
      WaitClick();
   }

   void NovelApp::SetSpeech(const Text& speech, const Character& character)
   {
      m_DialogBox->SetText(speech);
      m_DialogBox->SetCharacterName(character.GetName());
      m_DialogBox->SetCharacterNameColor(character.GetNameColor());
      SetHandler(new SpeechHandler(m_DialogBox));
      Loop();
   }

   void NovelApp::SetBackground(const char* imagePath)
   {
      m_Background = m_ResourceManager->Get<Texture>(imagePath);
   }

   void NovelApp::ChooseDialogOption(int dialogOptionIndex)
   {
      m_DialogBox->SetSelectedDialogOption(dialogOptionIndex);
      m_DialogBox->ClearDialogOptions();
      SetHandler(new ExitLoopHandler());
   }

   int NovelApp::SetDialogOptions(DialogOptions& dialogOptions)
   {
      m_DialogBox->SetText("");
      m_DialogBox->ClearDialogOptions();
      const Text* optionText;
      int i = 0;
      while((optionText = dialogOptions.GetOptionText()))
      {
         m_DialogBox->SetDialogOptionText(i, *optionText);
         i++;
         dialogOptions.Next();
      }
      SetHandler(new IdleHandler());
      Loop();
      if(!IsScriptPlaying())
         return -1;
      return m_DialogBox->GetSelectedDialogOption();
   }

   void NovelApp::Loop()
   {
      float frameTime = Timer::GetTimeSec(), 
            prevFrameTime = Timer::GetTimeSec();

      while(!m_Window->IsClosed())
      {
         if(m_CurrentHandler && m_CurrentHandler->IsDone())
         {
            SetHandler(nullptr);
            break;
         }

         OnUpdate(frameTime - prevFrameTime);
         m_Window->OnUpdate();

         prevFrameTime = frameTime;
         frameTime = Timer::GetTimeSec();
      }

      if(m_Window->IsClosed())
      {
         OnExit();
      }
   }

   void NovelApp::SetCanvasScale(const math::vec2f& scale)
   {
      m_Renderer->SetCanvasScale(scale);
   }

   const math::vec2f& NovelApp::GetCanvasScale() const
   {
      return m_Renderer->GetCanvasScale();
   }

   void NovelApp::OnEvent(Event& e)
   {
      EventDispatcher<NovelApp> d(this);

      d.Dispatch<WindowResizeEvent>(&NovelApp::OnWindowResize, e);
      d.Dispatch<MouseMoveEvent>(&NovelApp::OnMouseMove, e);
      d.Dispatch<MousePressedEvent>(&NovelApp::OnMousePressed, e);

      if(m_State == State::MainMenu)
      {
         m_MainMenu->OnEvent(e);
      }
      else if(m_State == State::PlayingScript)
      {
         m_PauseMenu->OnEvent(e);
         if(!m_IsPaused)
         {
            if(m_CurrentHandler)
               m_CurrentHandler->OnEvent(e);

            m_DialogBox->OnEvent(e);
         }
      }
   }

   void NovelApp::OnWindowResize(WindowResizeEvent& e)
   {
      m_Renderer->SetViewport(e.GetWidth(), e.GetHeight());
   }

   void NovelApp::OnMouseMove(MouseMoveEvent& e)
   {
      math::vec2f canvasCoords = ScreenToCanvas({ e.GetX(), e.GetY() });
      e.SetX(canvasCoords.x);
      e.SetY(canvasCoords.y);
   }

   void NovelApp::OnMousePressed(MousePressedEvent& /*e*/)
   {}

   math::vec2f
   NovelApp::ScreenToCanvas(const math::vec2f& screenCoords) const
   {
      return math::vec2f(
            GetCanvasScale().x * 
            (2 * screenCoords.x / m_Window->GetWidth() - 1),
            
            GetCanvasScale().y *
            (1  - 2 * screenCoords.y / m_Window->GetHeight()));
   }

   void NovelApp::OnUpdate(float deltaTime)
   {
      if(m_State == State::MainMenu)
      {
         m_MainMenu->OnUpdate(deltaTime);
         m_MainMenu->Draw(m_Renderer);
         return;
      }
      else if(m_State == State::PlayingScript)
      {
         if(m_Background)
         {
            Rect rect;
            rect.SetPos({0, 0})
               .SetScale(GetCanvasScale());
            m_Renderer->DrawRect(rect, *m_Background);
         }

         m_CharacterScene->Draw(m_Renderer);

         if(!m_IsPaused)
         {
            if(m_CurrentHandler)
               m_CurrentHandler->OnUpdate(deltaTime);

            m_DialogBox->OnUpdate(deltaTime);
         }

         m_DialogBox->Draw(m_Renderer);

         m_PauseMenu->OnUpdate(deltaTime);
         m_PauseMenu->Draw(m_Renderer);
      } 
   }

   void NovelApp::OnExit()
   {
      // Maybe do saving before exit in future
      LOG_INFO("Exiting...");

      delete m_PauseMenu;
      delete m_MainMenu;
      delete m_ScriptRegistry;

      if(m_Background)
         m_Background->Release();

      delete m_DialogBox;

      if(m_CurrentHandler)
         delete m_CurrentHandler;

      delete m_CharacterScene;

      delete m_Renderer;
      delete m_ResourceManager;
      delete m_Window;
      exit(0); 
   }
}
