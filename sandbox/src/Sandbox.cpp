#include <iostream>

#include "App.h"
#include "Log.h"
#include "Renderer/Renderer.h"
#include "Math/Vec.h"
#include "Animations/Animator.h"
#include "Animations/BasicAnimations.h"

enum { sq_count = 1000 };

class Sandbox : public App
{

public:
   Sandbox() : m_CurrPos(0)
   {
      m_Animator = new totem::Animator();
      m_Animator->PlayAnim(
         new totem::LinearAnim(m_ImageColor,
                              totem::math::vec4f(0, 0, 0, 0),
                              totem::math::vec4f(1, 1, 1, 1),
                              5)
                           );
      m_Animator->PlayAnim(
            new totem::LinearAnim(
                  m_ImagePos,
                  totem::math::vec2f(-0.7, -0.7),
                  totem::math::vec2f(0.8, 0.8),
                  10
               )        
      );
   }

   virtual void OnEvent(totem::Event& e) override
   {
      //LOG_INFO(e.ToString().c_str());
      if(e.GetType() == totem::EventType::MouseMove)
      {
         LOG_INFO(e.ToString().c_str());
         totem::MouseMoveEvent& me = CAST_EVENT(totem::MouseMoveEvent,e);
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
         totem::WindowResizeEvent& me = CAST_EVENT(totem::WindowResizeEvent,e);
         m_ScreenWidth = me.GetWidth();
         m_ScreenHeight = me.GetHeight();
      }

   }

   virtual void OnUpdate(float deltaTime) override
   {
      m_Animator->OnUpdate(deltaTime);
      //m_Renderer->Clear(0.2f, 0.3f, 0.2f);
      m_Renderer->DrawBackground("image.jpeg");
      //m_Renderer->DrawRect(totem::math::vec2f(-5, 1), 
      //                     totem::math::vec2f(5, 5));
      for(int i = 0; i < m_CurrPos; i++)
      {
         m_Renderer->DrawRect(m_Positions[i], 
                              totem::math::vec2f(0.7f, 0.7f),
                              totem::math::vec4f(0.4f, 0.6f, 0.3f, 0.1f));

      }
/*
      m_Renderer->DrawRect(totem::math::vec2f(0.0f, 0.0f),
                           totem::math::vec2f(14.0f, 7.0f),
                           "image.jpeg",
                           totem::math::vec4f(1, 1, 1, 0.5f));
*/
      m_Renderer->DrawImage("transp_image.png",
                           m_ImagePos,
                           3,
                           m_ImageColor);
   }

private:
   totem::math::vec2f m_Positions[sq_count];
   int m_CurrPos;
   float m_ScreenWidth, m_ScreenHeight;
   totem::Animator* m_Animator;
   totem::math::vec4f m_ImageColor;
   totem::math::vec2f m_ImagePos;
};

App* App::CreateApp()
{
   return new Sandbox();
}
