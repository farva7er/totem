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

class Sandbox;

class StartButtonListener : public totem::IClickListener
{
public:
   StartButtonListener(Sandbox* sandbox)
      : m_Sandbox(sandbox) {}

   void OnClick();

private:
   Sandbox* m_Sandbox;
};

class StopButtonListener : public totem::IClickListener
{
public:
   StopButtonListener(Sandbox* sandbox)
      : m_Sandbox(sandbox) {}

   void OnClick();

private:
   Sandbox* m_Sandbox;
};

class Sandbox : public App
{

public:
   Sandbox() : m_CurrPos(0), m_UIManager(m_Renderer)
   {
      m_ImageColor = totem::math::vec4f(1, 1, 1, 1);
      m_ColorAnim =
         new totem::LinearInterpAnim<totem::math::vec4f>
                                    (m_ImageColor,
                                    totem::math::vec4f(0, 0, 0, 0),
                                    totem::math::vec4f(1, 1, 1, 1),
                                    1);
      m_LbAnim =                          
            new totem::HermiteModifAnim<totem::math::vec2f>(
                  m_ImagePos,
                  totem::math::vec2f(-0.75f, -0.75f),
                  2
            );
      m_RtAnim =                          
            new totem::LinearModifAnim<totem::math::vec2f>(
                  m_ImagePos,
                  totem::math::vec2f(0.8f, 0.8f),
                  2
            );

      totem::Animation* rotAnim = new totem::HermiteInterpAnim<float>(
                                    m_Angle,
                                    0.0f,
                                    360.0f,
                                    3
                                 );
      m_Animator.Play(rotAnim);

      m_LbAnim2 = m_LbAnim->Clone();

      m_PartialAnimGroup.Add(m_ColorAnim);
      m_PartialAnimGroup.Add(m_LbAnim);
      m_PartialAnimGroup.Add(rotAnim);

      m_FullAnimGroup.Add(m_ColorAnim);
      m_FullAnimGroup.Add(m_LbAnim);
      m_FullAnimGroup.Add(m_RtAnim);
      m_FullAnimGroup.Add(m_LbAnim2);

      m_Animator.Add(m_FullAnimGroup);

      m_StartButton = m_UIManager.CreateButton(
                        totem::ButtonType::BoxButton,
                        new StartButtonListener(this));

      m_StartButton->SetPos(totem::math::vec2f(-0.5f, -0.5f));
      m_StartButton->SetScale(totem::math::vec2f(2, 1));
      m_StartButton->SetColor(totem::math::vec4f(0, 0.4f, 0.3f, 0.8f));
      m_StartButton->SetText("Start Anim");
 
      m_StopButton = m_UIManager.CreateButton(
                        totem::ButtonType::BoxButton,
                        new StopButtonListener(this));

      m_StopButton->SetPos(totem::math::vec2f(0.5f, -0.5f));
      m_StopButton->SetScale(totem::math::vec2f(2, 1));     
      m_StopButton->SetText("Stop Anim");

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

   void PlayTriangleAnim()
   {
      m_Animator.Play(m_PartialAnimGroup);
      m_Animator.Play(m_RtAnim, 2.0f, m_LbAnim);
      m_Animator.Play(m_LbAnim2, 1.0f, m_RtAnim);
   }

   void PauseTriangleAnim()
   {
      m_Animator.Pause(m_FullAnimGroup);
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
/*
      m_Renderer->DrawRect(totem::math::vec2f(0.0f, 0.0f),
                           totem::math::vec2f(14.0f, 7.0f),
                           "image.jpeg",
                           totem::math::vec4f(1, 1, 1, 0.5f));
*/
      m_Renderer->DrawImage("resources/transp_image.png",
                           m_ImagePos,
                           3,
                           m_ImageColor);

      totem::Rect textBgRect = totem::Rect::Builder()
                              .SetPos(totem::math::vec2f(0, 0.55f))
                              .SetScale(totem::math::vec2f(12, 4))
                              .SetColor(totem::math::vec4f(0, 0, 0, 0.5f))
                              .Construct();

      m_Renderer->DrawRect(textBgRect);

      totem::Rect rotatedRect = totem::Rect::Builder()
                           .SetScale(totem::math::vec2f(2.0f, 0.2f))
                           .SetColor(totem::math::vec4f(0, 0, 0, 0.7f))
                           .SetRotationAngle(m_Angle)
                           .SetRotationAxis(totem::math::vec2f(-1, 0))
                           .Construct();

      m_Renderer->DrawRect(rotatedRect);


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
   }

private:
   totem::math::vec2f m_Positions[sq_count];
   int m_CurrPos;
   float m_ScreenWidth, m_ScreenHeight;
   totem::Animator m_Animator;
   totem::math::vec4f m_ImageColor;
   totem::math::vec2f m_ImagePos;
   totem::Animation* m_ColorAnim;
   totem::Animation* m_LbAnim;
   totem::Animation* m_LbAnim2;
   totem::Animation* m_RtAnim;
   totem::AnimationGroup m_FullAnimGroup;
   totem::AnimationGroup m_PartialAnimGroup;
   totem::UIManager m_UIManager;
   totem::Button* m_StartButton;
   totem::Button* m_StopButton;
   float m_Angle;
};


void StartButtonListener::OnClick()
{
   m_Sandbox->PlayTriangleAnim();
}

void StopButtonListener::OnClick()
{
   m_Sandbox->PauseTriangleAnim();
}

App* App::CreateApp()
{
   return new Sandbox();
}
