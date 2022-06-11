#include <iostream>

#include "Novel/NovelApp.h"
#include "Log.h"
#include "Renderer/Renderer.h"
#include "Math/Vec.h"
#include "Animations/Animator.h"

#include "UI/UILinearLayout.h"
#include "UI/IIEListener.h"
#include "UI/Button.h"
#include "UI/CheckButton.h"
#include "UI/CommonMediators/UIButtonExclusiveGroup.h"
#include "Novel/DialogBox.h"

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
   Sandbox(int argc, char** argv) : totem::NovelApp(argc, argv), m_CurrPos(0)
   {
      SetCanvasScale(totem::math::vec2f(16, 9));
      SetBackground("resources/image.jpeg");

      m_TrianglePos = {0, 5};
      m_TriangleColor = {1, 1, 1, 1};
      m_ColorAnim =
         new totem::LinearInterpAnim<totem::math::vec4f>(
                                    totem::math::vec4f(0, 0, 0, 0),
                                    totem::math::vec4f(1, 1, 1, 1),
                                    3);

      m_Animator.Sync(m_ColorAnim, 5);


      m_WelcomeMsg = 
         "You're welcome to totem! It's a visual novel engine "
         "written in C++. Or at least it will be in future:) "
         "It's in development and supports little functionality, "
         "but it will support much more in near future. "
         "Добро пожаловать в totem!";

      m_DialogBox = new totem::DialogBox();
      m_DialogBox->SetScale({10, 2});
      m_DialogBox->SetPos({0, -7});
      m_DialogBox->SetText(m_WelcomeMsg);
      m_DialogBox->SetTextColor({ 0.9f, 0.9f, 0.2f, 1.0f });
      m_DialogBox->SetFontSize(0.6f);
      m_DialogBox->SetLineSpacing(1.5f);

      m_RootElement = m_DialogBox;
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

      if(e.GetType() == totem::EventType::MousePressed)
      {
         m_DialogBox->SetText(m_WelcomeMsg);
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
   totem::DialogBox* m_DialogBox;
   const char* m_WelcomeMsg;
};

totem::App* totem::App::CreateApp(int argc, char** argv)
{
   return new Sandbox(argc, argv);
}
