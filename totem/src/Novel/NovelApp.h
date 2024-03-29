#ifndef _TOTEM_NOVEL_APP_H_
#define _TOTEM_NOVEL_APP_H_

#include "Events.h"
#include "Math/Vec.h"
#include "Window.h"
#include "Internationalization/Text.h"
#include "DialogBox.h"
#include "MainMenu.h"
#include "PauseMenu.h"
#include "Character.h"
#include "CharacterScene.h"
#include "ScriptRegistry.h"
#include "DialogOptions.h"
#include "App.h"

namespace totem
{
   class NovelHandler
   {
      public:
         virtual ~NovelHandler() {}

         virtual bool IsDone() const = 0;
         virtual void OnUpdate(float deltaTime) = 0;
         virtual void OnEvent(Event& e) = 0;
   };

   class NovelApp : public App, public IEventListener
   {
      public:
         virtual ~NovelApp();

         static NovelApp* GetInstance();
         static ResourceManager* GetResourceManager();

         virtual void Run() = 0;
         void AddScript(Script& script);
         void PlayScript(int scriptIndex);

         void StartMainMenu();
         void ExitToMainMenu();
         bool IsScriptPlaying() const;

         void Pause();
         void UnPause();

         void WaitClick();
         void ClearDialogBox();
         void ShowCharacter(const Character& character, int slot);
         void HideCharacter(const Character& character);
         void SetSpeech(const Text& speech, const Character& character); 
         void SetBackground(const char* imagePath);
         void ChooseDialogOption(int optionIndex);
         
         int SetDialogOptions(DialogOptions& dialogOptions);

         void Loop();
         void OnExit();

      protected:
         NovelApp();

      private:
         virtual void OnEvent(Event& e) override;
         void OnWindowResize(WindowResizeEvent& e);
         void OnMouseMove(MouseMoveEvent& e);
         void OnMousePressed(MousePressedEvent& e);
         void OnUpdate(float deltaTime);

         void SetHandler(NovelHandler* handler);

         void SetCanvasScale(const math::vec2f& scale);
         const math::vec2f& GetCanvasScale() const;
         math::vec2f ScreenToCanvas(const math::vec2f& screenCoords) const;

      private:
         enum class State { MainMenu, PlayingScript };
         State m_State;
         bool m_IsPaused;

         static NovelApp* s_Instance;
         Window* m_Window;
         ResourceManager* m_ResourceManager;
         Renderer* m_Renderer;
         
         CharacterScene* m_CharacterScene;
         NovelHandler* m_CurrentHandler;
         DialogBox* m_DialogBox;
         Ref<Texture> m_Background;

         ScriptRegistry* m_ScriptRegistry;
         MainMenu* m_MainMenu;
         PauseMenu* m_PauseMenu;
   };

}

#endif
