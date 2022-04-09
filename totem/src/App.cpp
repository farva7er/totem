#include "App.h"
#include "Window.h"
#include "Renderer/Renderer.h"
#include "Timer.h"
#include "ResourceManager.h"

App::App()
{
   m_Window = totem::Window::Create(1280, 720, "Totem");
   m_Window->AddEventListener(this);
   m_Renderer = new totem::Renderer(m_Window);
   m_RootElement = nullptr;
}

App::~App()
{
   totem::ResourceManager::DeleteInstance();
   delete m_RootElement;
   delete m_Renderer;
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


void App::SetCanvasScale(const totem::math::vec2f& scale)
{
   m_Renderer->SetCanvasScale(scale);
}

const totem::math::vec2f& App::GetCanvasScale() const
{
   return m_Renderer->GetCanvasScale();
}

void App::OnEvent(totem::Event& e)
{
   totem::EventDispatcher<App> d(this);

   d.Dispatch<totem::WindowResizeEvent>(&App::OnWindowResize, e);
   d.Dispatch<totem::MouseMoveEvent>(&App::OnMouseMove, e);

   if(m_RootElement)
   {
      m_RootElement->OnEvent(e);
   }

   OnTotemEvent(e);
}

void App::OnWindowResize(totem::WindowResizeEvent& e)
{
   m_Renderer->SetViewport(e.GetWidth(), e.GetHeight());
}

void App::OnMouseMove(totem::MouseMoveEvent& e)
{
   totem::math::vec2f canvasCoords =
                           ScreenToCanvas({ e.GetX(), e.GetY() });
   e.SetX(canvasCoords.x);
   e.SetY(canvasCoords.y);
}

totem::math::vec2f
App::ScreenToCanvas(const totem::math::vec2f& screenCoords) const
{
   return totem::math::vec2f(
         GetCanvasScale().x * 
         (2 * screenCoords.x / m_Window->GetWidth() - 1),
         
         GetCanvasScale().y *
         (1  - 2 * screenCoords.y / m_Window->GetHeight()));
}

void App::OnUpdate(float deltaTime)
{
   if(m_Background)
      m_Renderer->DrawBackground(m_Background);

   if(m_RootElement)
   {
      m_RootElement->OnUpdate(deltaTime);
      m_RootElement->Draw(m_Renderer);
   }

   OnTotemUpdate(deltaTime);
}

