#include "App.h"
#include "Window.h"
#include "Renderer/Renderer.h"

App::App()
{
   m_Window = totem::Window::Create(640, 480, "Totem");
   m_Window->AddEventListener(this);
   m_Renderer = new totem::Renderer(m_Window);
}

App::~App()
{
   delete m_Renderer;
   delete m_Window;
}

void App::Run()
{
   while(!m_Window->IsClosed())
   {
      OnUpdate();
      m_Window->OnUpdate();
   }
}
