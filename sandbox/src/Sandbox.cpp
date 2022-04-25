#include <iostream>

#include "Novel/NovelApp.h"
#include "Log.h"
#include "Renderer/Renderer.h"
#include "Math/Vec.h"
#include "Animations/Animator.h"
#include "Animations/BasicAnimations.h"

#include "UI/UILinearLayout.h"
#include "UI/IIEListener.h"
#include "UI/Button.h"
#include "UI/CheckButton.h"

#include "Renderer/RendererPrimitives/Rect.h"

enum { sq_count = 1000 };

class ButtonListener : public totem::IIEListener
{
   virtual void OnClick(int button) override
   {
      LOG_INFO("Click: %d", button);
   }

   virtual void OnLostHover() override
   {
      LOG_INFO("LostHover");
   }

   virtual void OnPush() override
   {
      LOG_INFO("Push");
   }

   virtual void OnHover() override
   {
      LOG_INFO("Hover");
   }

   virtual totem::IIEListener* Clone() const
   {
      return new ButtonListener();
   }
};

class Sandbox : public totem::NovelApp
{

public:
   Sandbox() : m_CurrPos(0)
   {
      SetCanvasScale(totem::math::vec2f(16, 9));
      SetBackground("resources/image.jpeg");

      m_TrianglePos = totem::math::vec2f(-10, 5);
      m_TriangleColor = totem::math::vec4f(1, 1, 1, 1);
      m_ColorAnim =
         new totem::LinearInterpAnim<totem::math::vec4f>(
                                    totem::math::vec4f(0, 0, 0, 0),
                                    totem::math::vec4f(1, 1, 1, 1),
                                    3);

      //m_ColorAnim->Play();
      //m_Animator.Add(m_ColorAnim);
      m_Animator.Sync(m_ColorAnim, 5);



      totem::UILinearLayout* layout = new totem::UILinearLayout();
      layout->SetScale(totem::math::vec2f(4, 0));
      layout->SetPos(totem::math::vec2f(0, 4));
      layout->SetSpacing(0.1f);


      totem::IButton* sButton = new totem::BoxButton();
      sButton->SetColor(totem::math::vec4f(0.3f, 0.8f, 0.8f, 1.0f));
      sButton->AddListener(new ButtonListener());
      sButton->SetText("Simple l1");

      totem::IButton* fButton = new totem::ButtonColorDecorator(
                                          new totem::BoxButton());
      fButton->SetColor(totem::math::vec4f(0.3f, 0.1f, 0.8f, 1.0f));
      fButton->AddListener(new ButtonListener());
      fButton->SetText("Fixed l1");

      totem::IButton* soButton = new totem::ButtonScaleDecorator(
                                    new totem::ButtonColorDecorator(
                                       new totem::BoxButton()));
      soButton->SetColor(totem::math::vec4f(0.6f, 0.2f, 0.3f, 1.0f));
      soButton->AddListener(new ButtonListener());
      soButton->SetText("Soft l1");

      layout->AddElement(sButton);
      layout->AddElement(fButton);
      layout->AddElement(soButton);

      totem::UILinearLayout* layout2 = new totem::UILinearLayout();
      layout2->SetScale(totem::math::vec2f(4, 0));
      layout2->SetSpacing(0.4f);

      totem::IButton* fButton2 = new totem::ButtonColorDecorator(
                                          new totem::BoxButton());
      fButton2->SetColor(totem::math::vec4f(0.3f, 0.1f, 0.8f, 1.0f));
      fButton2->AddListener(new ButtonListener());
      fButton2->SetText("Fixed l2");

      totem::IButton* soButton2 = new totem::ButtonScaleDecorator(
                                    new totem::ButtonColorDecorator(
                                       new totem::BoxButton()));
      soButton2->SetColor(totem::math::vec4f(0.6f, 0.2f, 0.3f, 1.0f));
      soButton2->AddListener(new ButtonListener());
      soButton2->SetText("Soft l2");

      layout2->AddElement(fButton2);
      //layout2->AddElement(soButton2);
      
      layout->AddElement(layout2);
      delete soButton2;

      totem::CheckButton* checkButton = new totem::CheckButton();
      checkButton->SetPos({ 0, 8 });
      checkButton->SetColor({ 0.1f, 0.1f, 0.1f, 0.8f });
      //checkButton->SetActive(false);
      checkButton->SetChecked(true);
      totem::IButton* chckButton = 
         new totem::ButtonScaleDecorator(checkButton);
      layout2->AddElement(chckButton);
      fButton2->SetActive(false);
      m_RootElement = layout;
   }

   virtual void OnTotemEvent(totem::Event& e) override
   {
      if(e.GetType() == totem::EventType::MouseMove)
      {
         totem::MouseMoveEvent& me = e.Cast<totem::MouseMoveEvent>();

         m_Positions[m_CurrPos].x = me.GetX();
         m_Positions[m_CurrPos].y = me.GetY();

         m_CurrPos++;
         if(m_CurrPos >= sq_count)
            m_CurrPos = 0;
      }

      if(e.GetType() == totem::EventType::WindowResize)
      {
         totem::WindowResizeEvent& wre = e.Cast<totem::WindowResizeEvent>();
         m_ScreenWidth = wre.GetWidth();
         m_ScreenHeight = wre.GetHeight();
      } 
   }

   virtual void OnTotemUpdate(float deltaTime) override
   {

      //LOG_INFO("%f", deltaTime);
      m_Animator.OnUpdate(deltaTime);
      //m_Renderer->Clear(0.2f, 0.3f, 0.2f);
      for(int i = 0; i < m_CurrPos; i++)
      {
         totem::Rect rect = totem::Rect::Builder()
                        .SetPos(m_Positions[i])
                        .SetScale(totem::math::vec2f(0.2f, 0.2f))
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
};

totem::App* totem::App::CreateApp()
{
   return new Sandbox();
}
