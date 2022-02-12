#ifndef _TOTEM_APP_H_
#define _TOTEM_APP_H_

#include "Events.h"
#include "Window.h"
#include "Renderer/Renderer.h"

class App : public totem::EventListener {

public:
   App();
   virtual ~App();

   // To Be defined by sandbox
   static App* CreateApp();

   void Run();
   virtual void OnUpdate(float deltaTime) {}
   virtual void OnEvent(totem::Event& e) override {}

private:
   totem::Window* m_Window;
protected:
   totem::Renderer* m_Renderer;
};

#endif
