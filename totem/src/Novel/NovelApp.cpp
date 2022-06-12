#include "NovelApp.h"
#include "Window.h"
#include "Renderer/Renderer.h"
#include "Timer.h"
#include "ResourceManager.h"

#include <stdlib.h>


namespace totem
{
   NovelApp* NovelApp::s_Instance = nullptr;

   NovelApp* NovelApp::GetInstance()
   {
      TOTEM_ASSERT(s_Instance, "Novel App is not created!!!")
      return s_Instance;
   }

   NovelApp::NovelApp()
      : App(0, nullptr)
   {
      m_Window = Window::Create(1280, 720, "Totem");
      m_Window->AddEventListener(this);
      m_Renderer = new Renderer(m_Window);
   
      SetCanvasScale({ 16, 9 });

      m_Background = nullptr;

      m_DialogBox = new totem::DialogBox();
      m_DialogBox->SetScale({10, 2});
      m_DialogBox->SetPos({0, -7});
      m_DialogBox->SetTextColor({ 0.9f, 0.9f, 0.2f, 1.0f });
      m_DialogBox->SetFontSize(0.6f);
      m_DialogBox->SetLineSpacing(1.5f);

      m_RootElement = m_DialogBox;

      m_Window->SendInitEvents();

      s_Instance = this;
   }

   NovelApp::~NovelApp()
   {
      OnExit();
   }

   void NovelApp::SetSpeech(const Text& speech /*, TODO Character*/)
   {
      m_DialogBox->SetText(speech);
      Loop();
   }

   void NovelApp::SetBackground(const char* imagePath)
   {
      // BAAAAAD!!!!!!!!!!!!!!!!!!
      m_Background = imagePath;
   }

   void NovelApp::Loop()
   {
      float frameTime = Timer::GetTimeSec(), 
            prevFrameTime = Timer::GetTimeSec();

      m_LoopShouldExit = false;
      while(!m_Window->IsClosed() && !m_LoopShouldExit)
      {
         OnUpdate(frameTime - prevFrameTime);
         m_Window->OnUpdate();

         prevFrameTime = frameTime;
         frameTime = Timer::GetTimeSec();
      }

      if(m_Window->IsClosed())
      {
         OnExit();
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
      d.Dispatch<MousePressedEvent>(&NovelApp::OnMousePressed, e);

      if(m_RootElement)
      {
         m_RootElement->OnEvent(e);
      }

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

   void NovelApp::OnMousePressed(MousePressedEvent& e)
   {
      if(e.GetButton() == 0)
      {
         NextCall();
      }
   }

   void NovelApp::NextCall()
   {
      m_LoopShouldExit = true;
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
   }

   void NovelApp::OnExit()
   {
      // Maybe do saving before exit in future
      LOG_INFO("Exiting...");
      exit(0); 
   }
}

