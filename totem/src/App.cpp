#include "App.h"
#include "Window.h"
#include "Renderer/Renderer.h"
#include "Timer.h"
#include "ResourceManager.h"


namespace totem
{
   App::App()
   {
      m_Window = Window::Create(1280, 720, "Totem");
      m_Window->AddEventListener(this);
      m_Renderer = new Renderer(m_Window);
      m_RootElement = nullptr;
   }

   App::~App()
   {
      ResourceManager::DeleteInstance();
      delete m_RootElement;
      delete m_Renderer;
      delete m_Window;
   }

   void App::Run()
   {
      float frameTime = Timer::GetTimeSec(), 
            prevFrameTime = Timer::GetTimeSec();

      m_Window->SendInitEvents();
      while(!m_Window->IsClosed())
      {
         OnUpdate(frameTime - prevFrameTime);
         m_Window->OnUpdate();

         prevFrameTime = frameTime;
         frameTime = Timer::GetTimeSec();
      }
   }


   void App::SetCanvasScale(const math::vec2f& scale)
   {
      m_Renderer->SetCanvasScale(scale);
   }

   const math::vec2f& App::GetCanvasScale() const
   {
      return m_Renderer->GetCanvasScale();
   }

   void App::OnEvent(Event& e)
   {
      EventDispatcher<App> d(this);

      d.Dispatch<WindowResizeEvent>(&App::OnWindowResize, e);
      d.Dispatch<MouseMoveEvent>(&App::OnMouseMove, e);

      if(m_RootElement)
      {
         m_RootElement->OnEvent(e);
      }

      OnTotemEvent(e);
   }

   void App::OnWindowResize(WindowResizeEvent& e)
   {
      m_Renderer->SetViewport(e.GetWidth(), e.GetHeight());
   }

   void App::OnMouseMove(MouseMoveEvent& e)
   {
      math::vec2f canvasCoords =
                              ScreenToCanvas({ e.GetX(), e.GetY() });
      e.SetX(canvasCoords.x);
      e.SetY(canvasCoords.y);
   }

   math::vec2f
   App::ScreenToCanvas(const math::vec2f& screenCoords) const
   {
      return math::vec2f(
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
}

