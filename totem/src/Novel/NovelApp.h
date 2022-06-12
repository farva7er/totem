#ifndef _TOTEM_NOVEL_APP_H_
#define _TOTEM_NOVEL_APP_H_

#include "Events.h"
#include "Math/Vec.h"
#include "Window.h"
#include "UI/IUIElement.h"
#include "Internationalization/Text.h"
#include "DialogBox.h"
#include "App.h"

namespace totem
{
   class NovelApp : public App, public IEventListener
   {
   public:
      virtual ~NovelApp();
      static NovelApp* GetInstance();
      virtual void Run() = 0;

      /* Utility functions for user calls */
      void SetSpeech(const Text& speech /*, TODO Character& ch */); 
      void SetBackground(const char* imagePath);
      
      /////////////////

      // Enter the loop and maybe wait for user interaction
      void Loop();

      // Get Next user call and execute it
      void NextCall();

   protected:
      NovelApp();

   private:
      virtual void OnEvent(Event& e) override;
      void OnWindowResize(WindowResizeEvent& e);
      void OnMouseMove(MouseMoveEvent& e);
      void OnMousePressed(MousePressedEvent& e);
      void OnUpdate(float deltaTime);
      void OnExit();

      void SetCanvasScale(const math::vec2f& scale);
      const math::vec2f& GetCanvasScale() const;
      math::vec2f ScreenToCanvas(const math::vec2f& screenCoords) const;

   private:
      static NovelApp* s_Instance;
      Window* m_Window;
      bool m_LoopShouldExit;
   protected:
      Renderer* m_Renderer;
      IUIElement* m_RootElement;
      DialogBox* m_DialogBox;
      const char* m_Background;
   };
}

#endif
