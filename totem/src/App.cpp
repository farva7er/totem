#include "App.h"
#include "Window.h"
#include "Renderer/Renderer.h"
#include "Timer.h"

App::App()
{
   m_Window = totem::Window::Create(640, 480, "Totem");
   m_Window->AddEventListener(this);
   m_Renderer = new totem::Renderer(m_Window);
}

App::~App()
{
   delete m_Window;
}

void App::Run()
{
   float frameTime = totem::Timer::GetTimeSec(), 
         prevFrameTime = totem::Timer::GetTimeSec();
   m_Window->SendInitEvents();
   while(!m_Window->IsClosed())
   {
      OnUpdate(frameTime - prevFrameTime);
      m_Window->OnUpdate();

      prevFrameTime = frameTime;
      frameTime = totem::Timer::GetTimeSec();
   }
}
