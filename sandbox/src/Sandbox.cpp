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

class ButtonListener : public totem::IButtonListener
{
   virtual void OnClick() override
   {
      LOG_INFO("Click");
   }

   virtual void OnIdle() override
   {
      LOG_INFO("Idle");
   }

   virtual void OnPush() override
   {
      LOG_INFO("Push");
   }

   virtual void OnHover() override
   {
      LOG_INFO("Hover");
   }
};

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

      totem::IButton* button = m_UIManager
         .CreateButton(totem::ButtonType::AnimatedBoxButton);
      button->SetPos(totem::math::vec2f(5, 0));
      button->SetScale(totem::math::vec2f(2, 1));
      button->SetColor(totem::math::vec4f(0.3f, 0.1f, 0.8f, 1.0f));
      button->AddListener(new ButtonListener());
      button->SetText("No, Click me");

      totem::IButton* animButton = m_UIManager
         .CreateButton(totem::ButtonType::BoxButton);
      animButton->SetPos(totem::math::vec2f(-5, 0));
      animButton->SetScale(totem::math::vec2f(2, 1));
      animButton->SetColor(totem::math::vec4f(0.6f, 0.2f, 0.3f, 1.0f));
      animButton->AddListener(new ButtonListener());
      animButton->SetText("Click me");


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

         totem::math::vec2f sceneSizes  = m_Renderer->GetSceneSize();
         m_Positions[m_CurrPos].x *= sceneSizes.x;
         m_Positions[m_CurrPos].y *= sceneSizes.y;

         m_CurrPos++;
         if(m_CurrPos >= sq_count)
            m_CurrPos = 0;
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

      totem::math::vec2f textBgRectPos(0, 5.5f);
      totem::math::vec2f textBgRectScale(12, 4);

      totem::Rect textBgRect = totem::Rect::Builder()
                              .SetPos(textBgRectPos)
                              .SetScale(textBgRectScale)
                              .SetColor(totem::math::vec4f(0, 0, 0, 0.5f))
                              .Construct();

      m_Renderer->DrawRect(textBgRect);


      m_Renderer->
         DrawControlledText("Welcome to totem!",
               totem::math::vec2f(textBgRectPos.x, textBgRectPos.y + 2),
               totem::math::vec2f(textBgRectScale.x, textBgRectScale.y * 0.5),
               2.5f,
               totem::math::vec4f(0.6f, 0.7f, 0.6f, 0.8f),
               totem::TextAlign::HCenter | totem::TextAlign::VCenter);


      m_Renderer->
         DrawControlledText("visual novel engine",
               totem::math::vec2f(textBgRectPos.x, textBgRectPos.y - 1),
               totem::math::vec2f(textBgRectScale.x, textBgRectScale.y * 0.25),
               1.5f,
               totem::math::vec4f(0.6f, 0.7f, 0.6f, 0.8f),
               totem::TextAlign::HCenter | totem::TextAlign::VCenter);

      m_Renderer->
         DrawControlledText("hopefully, in future ;)",
               totem::math::vec2f(textBgRectPos.x, textBgRectPos.y - 3),
               totem::math::vec2f(textBgRectScale.x, textBgRectScale.y * 0.25),
               1.0f,
               totem::math::vec4f(0.6f, 0.7f, 0.6f, 0.8f),
               totem::TextAlign::HCenter | totem::TextAlign::VCenter);

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
