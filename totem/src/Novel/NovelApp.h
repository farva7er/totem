#ifndef _TOTEM_NOVEL_APP_H_
#define _TOTEM_NOVEL_APP_H_

#include "Events.h"
#include "Math/Vec.h"
#include "Window.h"
#include "Internationalization/Text.h"
#include "DialogBox.h"
#include "Character.h"
#include "CharacterScene.h"
#include "ScriptRegistry.h"
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

   class MainMenu;

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

         void ShowCharacter(const Character& character, int slot);
         void HideCharacter(const Character& character);
         void SetSpeech(const Text& speech, const Character& character); 
         void SetBackground(const char* imagePath);
         
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

         static NovelApp* s_Instance;
         Window* m_Window;
         Renderer* m_Renderer;
         ResourceManager* m_ResourceManager;
         
         CharacterScene m_CharacterScene;
         NovelHandler* m_CurrentHandler;
         DialogBox* m_DialogBox;
         Ref<Texture> m_Background;

         ScriptRegistry m_ScriptRegistry;
         MainMenu* m_MainMenu;
   };

}

#endif
