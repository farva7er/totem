#include "NovelApp.h"
#include "Window.h"
#include "Renderer/Renderer.h"
#include "Timer.h"
#include "ResourceManager.h"


namespace totem
{
   NovelApp::NovelApp()
   {
      m_Window = Window::Create(1280, 720, "Totem");
      m_Window->AddEventListener(this);
      m_Renderer = new Renderer(m_Window);
      m_RootElement = nullptr;
   }

   NovelApp::~NovelApp()
   {
      ResourceManager::DeleteInstance();
      delete m_RootElement;
      delete m_Renderer;
      delete m_Window;
   }

   void NovelApp::Run()
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


   void NovelApp::SetCanvasScale(const math::vec2f& scale)
   {
      m_Renderer->SetCanvasScale(scale);
   }

   const math::vec2f& NovelApp::GetCanvasScale() const
   {
      return m_Renderer->GetCanvasScale();
   }

   void NovelApp::OnEvent(Event& e)
   {
      EventDispatcher<NovelApp> d(this);

      d.Dispatch<WindowResizeEvent>(&NovelApp::OnWindowResize, e);
      d.Dispatch<MouseMoveEvent>(&NovelApp::OnMouseMove, e);

      if(m_RootElement)
      {
         m_RootElement->OnEvent(e);
      }

      OnTotemEvent(e);
   }

   void NovelApp::OnWindowResize(WindowResizeEvent& e)
   {
      m_Renderer->SetViewport(e.GetWidth(), e.GetHeight());
   }

   void NovelApp::OnMouseMove(MouseMoveEvent& e)
   {
      math::vec2f canvasCoords =
                              ScreenToCanvas({ e.GetX(), e.GetY() });
      e.SetX(canvasCoords.x);
      e.SetY(canvasCoords.y);
   }

   math::vec2f
   NovelApp::ScreenToCanvas(const math::vec2f& screenCoords) const
   {
      return math::vec2f(
            GetCanvasScale().x * 
            (2 * screenCoords.x / m_Window->GetWidth() - 1),
            
            GetCanvasScale().y *
            (1  - 2 * screenCoords.y / m_Window->GetHeight()));
   }

   void NovelApp::OnUpdate(float deltaTime)
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

