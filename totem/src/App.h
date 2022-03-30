#ifndef _TOTEM_APP_H_
#define _TOTEM_APP_H_

#include "Events.h"
#include "Window.h"
#include "Renderer/Renderer.h"
#include "UI/IUIElement.h"

class App : public totem::IEventListener
{
public:
   App();
   virtual ~App();

   // To Be defined by sandbox
   static App* CreateApp();

   void Run();
   virtual void OnTotemEvent(totem::Event& e) = 0;
   virtual void OnTotemUpdate(float deltaTime) = 0;

   void SetCanvasScale(const totem::math::vec2f& scale);
   const totem::math::vec2f& GetCanvasScale() const;
   void SetBackground(const char* imagePath)
   { m_Background = imagePath; }

private:
   virtual void OnEvent(totem::Event& e) override;
   virtual void OnUpdate(float deltaTime);

   totem::math::vec2f
   ScreenToCanvas(const totem::math::vec2f& screenCoords) const;

private:
   totem::Window* m_Window;
protected:
   totem::Renderer* m_Renderer;
   totem::IUIElement* m_RootElement;
   const char* m_Background;
};

#endif
