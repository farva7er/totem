#include "App.h"
#include "Window.h"
#include "Renderer/Renderer.h"
#include "Timer.h"
#include "ResourceManager.h"

App::App()
{
   m_Window = totem::Window::Create(640, 480, "Totem");
   m_Window->AddEventListener(this);
   m_Renderer = new totem::Renderer(m_Window);
   m_Window->SendInitEvents();
}

App::~App()
{
   totem::ResourceManager::DeleteInstance();
   delete m_Renderer;
   delete m_Window;
}

void App::Run()
{
   float frameTime = totem::Timer::GetTimeSec(), 
         prevFrameTime = totem::Timer::GetTimeSec();
   while(!m_Window->IsClosed())
   {
      OnUpdate(frameTime - prevFrameTime);
      m_Window->OnUpdate();

      prevFrameTime = frameTime;
      frameTime = totem::Timer::GetTimeSec();
   }
}
