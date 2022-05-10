#ifndef _TOTEM_NOVEL_APP_H_
#define _TOTEM_NOVEL_APP_H_

#include "Events.h"
#include "Math/Vec.h"
#include "Window.h"
#include "UI/IUIElement.h"
#include "App.h"

namespace totem
{
   class NovelApp : public App, public IEventListener
   {
   public:
      NovelApp(int argc, char** argv);
      virtual ~NovelApp();

      virtual void Run() override;
      virtual void OnTotemEvent(Event& e) = 0;
      virtual void OnTotemUpdate(float deltaTime) = 0;

      void SetCanvasScale(const math::vec2f& scale);
      const math::vec2f& GetCanvasScale() const;
      void SetBackground(const char* imagePath)
      { m_Background = imagePath; }

   private:
      virtual void OnEvent(Event& e) override;
      void OnWindowResize(WindowResizeEvent& e);
      void OnMouseMove(MouseMoveEvent& e);
      void OnUpdate(float deltaTime);

      math::vec2f ScreenToCanvas(const math::vec2f& screenCoords) const;

   private:
      Window* m_Window;
   protected:
      Renderer* m_Renderer;
      IUIElement* m_RootElement;
      const char* m_Background;
   };
}

#endif
