#ifndef _TOTEM_NOVEL_APP_H_
#define _TOTEM_NOVEL_APP_H_

#include "Events.h"
#include "Math/Vec.h"
#include "Window.h"
#include "Internationalization/Text.h"
#include "DialogBox.h"
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
         virtual void Run() = 0;

         void SetSpeech(const Text& speech /*, TODO Character& ch */); 
         void SetBackground(const char* imagePath);
         
         void Loop();

      protected:
         NovelApp();

      private:
         virtual void OnEvent(Event& e) override;
         void OnWindowResize(WindowResizeEvent& e);
         void OnMouseMove(MouseMoveEvent& e);
         void OnMousePressed(MousePressedEvent& e);
         void OnUpdate(float deltaTime);
         void OnExit();

         void SetHandler(NovelHandler* handler);

         void SetCanvasScale(const math::vec2f& scale);
         const math::vec2f& GetCanvasScale() const;
         math::vec2f ScreenToCanvas(const math::vec2f& screenCoords) const;

      private:
         static NovelApp* s_Instance;
         Window* m_Window;
         ResourceManager* m_ResourceManager;
         NovelHandler* m_CurrentHandler;
      protected:
         Renderer* m_Renderer;
         DialogBox* m_DialogBox;
         Ref<Texture> m_Background;
   };

}

#endif
