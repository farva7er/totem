#include <iostream>

#include "App.h"
#include "Log.h"
#include "Renderer/Renderer.h"
#include "Math/Vec.h"
#include "Animations/Animator.h"
#include "Animations/BasicAnimations.h"

#include "UI/UIManager.h"

#include "Renderer/RendererPrimitives/Rect.h"

enum { sq_count = 1000 };

class Sandbox : public App
{

public:
   Sandbox() : m_CurrPos(0), m_UIManager(m_Renderer)
   {
      m_TriangleColor = totem::math::vec4f(1, 1, 1, 1);
      m_ColorAnim =
         new totem::LinearInterpAnim<totem::math::vec4f>(
                                    totem::math::vec4f(0, 0, 0, 0),
                                    totem::math::vec4f(1, 1, 1, 1),
                                    3);

      //m_ColorAnim->Play();
      //m_Animator.Add(m_ColorAnim);
      m_Animator.Sync(m_ColorAnim, 5);
   }

   virtual void OnEvent(totem::Event& e) override
   {
      //LOG_INFO(e.ToString().c_str());
      if(e.GetType() == totem::EventType::MouseMove)
      {
         //LOG_INFO(e.ToString().c_str());
         totem::MouseMoveEvent& me = e.Cast<totem::MouseMoveEvent>();
         m_Positions[m_CurrPos] = 2 * 
                        totem::math::vec2f(me.GetX()/m_ScreenWidth - 0.5f,
                                          -me.GetY()/m_ScreenHeight + 0.5f);
         m_CurrPos++;
         if(m_CurrPos >= sq_count)
            m_CurrPos = 0;
         //m_Renderer->Clear(me.GetX()/1000.0f, me.GetY()/1000.0f, 0.2f);
      }
      if(e.GetType() == totem::EventType::WindowResize)
      {
         totem::WindowResizeEvent& me = e.Cast<totem::WindowResizeEvent>();
         m_ScreenWidth = me.GetWidth();
         m_ScreenHeight = me.GetHeight();
      }
 
      m_UIManager.OnEvent(e);
   }

   virtual void OnUpdate(float deltaTime) override
   {

      //LOG_INFO("%f", deltaTime);
      m_Animator.OnUpdate(deltaTime);
      //m_Renderer->Clear(0.2f, 0.3f, 0.2f);
      m_Renderer->DrawBackground("resources/image.jpeg");
      for(int i = 0; i < m_CurrPos; i++)
      {
         totem::Rect rect = totem::Rect::Builder()
                        .SetPos(m_Positions[i])
                        .SetScale(totem::math::vec2f(0.7f, 0.7f))
                        .SetColor(totem::math::vec4f(0.4f, 0.6f, 0.4f, 0.1f))
                        .Construct();

         m_Renderer->DrawRect(rect);

      }

      m_Renderer->DrawImage("resources/transp_image.png",
                           m_TrianglePos,
                           3,
                           m_TriangleColor);

      totem::Rect textBgRect = totem::Rect::Builder()
                              .SetPos(totem::math::vec2f(0, 0.55f))
                              .SetScale(totem::math::vec2f(12, 4))
                              .SetColor(totem::math::vec4f(0, 0, 0, 0.5f))
                              .Construct();

      m_Renderer->DrawRect(textBgRect);


      m_Renderer->DrawText("Welcome to totem!",
                           totem::math::vec2f(-0.5f, 0.5f),
                           2.5f,
                           totem::math::vec4f(0.6f, 0.7f, 0.6f, 0.8f));

      m_Renderer->DrawText("visual novel engine",
                           totem::math::vec2f(-0.35f, 0.4f),
                           1.5f,
                           totem::math::vec4f(0.4f, 0.8f, 0.4f, 0.8f));

      m_Renderer->DrawText("hopefully, in future ;)",
                           totem::math::vec2f(-0.2f, 0.3f),
                           1,
                           totem::math::vec4f(0.4f, 0.8f, 0.4f, 0.8f));
      //LOG_INFO("deltaTime: %f", deltaTime);

      m_UIManager.OnUpdate(deltaTime);
      m_ColorAnim->ApplyVal(m_TriangleColor);
   }

private:
   totem::math::vec2f m_Positions[sq_count];
   int m_CurrPos;
   float m_ScreenWidth, m_ScreenHeight;
   totem::Animator m_Animator;
   totem::math::vec4f m_TriangleColor;
   totem::math::vec2f m_TrianglePos;
   totem::InterpAnim<totem::math::vec4f>* m_ColorAnim;
   totem::UIManager m_UIManager;
};

App* App::CreateApp()
{
   return new Sandbox();
}
