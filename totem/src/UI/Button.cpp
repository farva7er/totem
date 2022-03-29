#include <string.h>

#include "Button.h"

namespace totem
{
   ////////////////////////////////////////////////////////////////////
   /////         BoxButton       //////////////////////////////////////
   ////////////////////////////////////////////////////////////////////
   BoxButton::BoxButton()
      : m_Text(nullptr), m_Color(math::vec4f(0, 0, 0, 1))
   {}

   void BoxButton::Draw(Renderer* renderer) const
   {
      Rect rect = Rect::Builder()
                        .SetPos(m_Pos)
                        .SetScale(m_Scale)
                        .SetColor(m_Color)
                        .Construct();

      renderer->DrawRect(rect);
      if(m_Text)
      { 
         renderer->DrawControlledText(
                     m_Text, m_Pos, m_Scale, m_Scale.y * 0.7f,
                     math::vec4f(1, 1, 1, 1),
                     TextAlign::VCenter | TextAlign::HCenter
                     );
      }
   }

   bool BoxButton::IsHovered(const math::vec2f& cursorCoords) const
   {
      if(cursorCoords.x >= m_Pos.x - m_Scale.x &&
         cursorCoords.x <= m_Pos.x + m_Scale.x &&
         cursorCoords.y >= m_Pos.y - m_Scale.y &&
         cursorCoords.y <= m_Pos.y + m_Scale.y)
      {
         return true;
      }
      return false;
   }

   void BoxButton::SetText(const char* text)
   {
      if(m_Text)
         delete [] m_Text;

      if(!text)
      {
         m_Text = nullptr;
         return;
      }

      m_Text = new char[strlen(text) + 1];
      strcpy(m_Text, text);
   }

//////////////////////////////////////////////////////////////
////////         ButtonBaseDecorator      ////////////////////
//////////////////////////////////////////////////////////////

   void ButtonBaseDecorator::Draw(Renderer* renderer) const
   {
      m_Wrapee->Draw(renderer);
   }

   void ButtonBaseDecorator::AddListener(IUIElementListener* listener)
   {
      m_Wrapee->AddListener(listener);
   }

//////////////////////////////////////////////////////////////
////////       ButtonAnimDecorator     ///////////////////////
//////////////////////////////////////////////////////////////

   ButtonAnimDecorator::ButtonAnimDecorator(IButton* button)
      : ButtonBaseDecorator(button)
   {
      m_BaseScale = button->GetScale();
      m_BaseColor = button->GetColor();


      m_LostHoverAnimDuration = 0.3f;
      m_HoverAnimDuration = 0.2f;
      m_PushAnimDuration = 0.1f;

      m_HoverScaleFactor = 1.1f;
      m_PushScaleFactor = 1.05f;

      m_LostHoverColorAlpha = 0.7f;
      m_HoverColorAlpha = 0.9f;
      m_PushColorAlpha = 1.0f;

      m_LostHoverScaleAnim = new HermiteInterpAnim<math::vec2f>(
                                 math::vec2f(0, 0),
                                 m_BaseScale,
                                 m_LostHoverAnimDuration);

      m_HoverScaleAnim = new HermiteInterpAnim<math::vec2f>(
                                 math::vec2f(0, 0),
                                 m_BaseScale * m_HoverScaleFactor,
                                 m_HoverAnimDuration);

      m_PushScaleAnim = new HermiteInterpAnim<math::vec2f>(
                                 math::vec2f(0, 0),
                                 m_BaseScale * m_PushScaleFactor,
                                 m_PushAnimDuration);



      m_LostHoverColorAnim = new HermiteInterpAnim<math::vec4f>(
                                 math::vec4f(0, 0, 0, 0),
                                 math::vec4f(m_BaseColor.x,
                                             m_BaseColor.y,
                                             m_BaseColor.z,
                                             m_LostHoverColorAlpha),
                                 m_LostHoverAnimDuration);

      m_HoverColorAnim = new HermiteInterpAnim<math::vec4f>(
                                 math::vec4f(0, 0, 0, 0),
                                 math::vec4f(m_BaseColor.x,
                                             m_BaseColor.y,
                                             m_BaseColor.z,
                                             m_HoverColorAlpha),
                                 m_HoverAnimDuration);

      m_PushColorAnim = new HermiteInterpAnim<math::vec4f>(
                                 math::vec4f(0, 0, 0, 0),
                                 math::vec4f(m_BaseColor.x,
                                             m_BaseColor.y,
                                             m_BaseColor.z,
                                             m_PushColorAlpha),
                                 m_PushAnimDuration);

      m_AllAnimations.Add(m_LostHoverScaleAnim);
      m_AllAnimations.Add(m_HoverScaleAnim);
      m_AllAnimations.Add(m_PushScaleAnim);

      m_AllAnimations.Add(m_LostHoverColorAnim);
      m_AllAnimations.Add(m_HoverColorAnim);
      m_AllAnimations.Add(m_PushColorAnim);

      m_Animator.Add(m_AllAnimations);

      button->AddListener(new ButtonListener(this));
   }

   void ButtonAnimDecorator::OnUpdate(float deltaTime)
   {
      m_Animator.OnUpdate(deltaTime);
      math::vec2f scale;
      m_LostHoverScaleAnim->ApplyVal(scale);
      m_HoverScaleAnim->ApplyVal(scale);
      m_PushScaleAnim->ApplyVal(scale);

      if(!m_LostHoverScaleAnim->IsPaused() || !m_HoverScaleAnim->IsPaused() ||
         !m_PushScaleAnim->IsPaused())
      {
         ButtonBaseDecorator::SetScale(scale);
      }

      math::vec4f color;

      m_LostHoverColorAnim->ApplyVal(color);
      m_HoverColorAnim->ApplyVal(color);
      m_PushColorAnim->ApplyVal(color);

      if(!m_LostHoverColorAnim->IsPaused() || !m_HoverColorAnim->IsPaused() ||
         !m_PushColorAnim->IsPaused())
      {
         ButtonBaseDecorator::SetColor(color);
      }
   }

   void ButtonAnimDecorator::SetScale(const math::vec2f& scale)
   {
      ButtonBaseDecorator::SetScale(scale);
      m_BaseScale = scale;
      m_LostHoverScaleAnim->SetFinVal(scale);
      m_HoverScaleAnim->SetFinVal(scale * m_HoverScaleFactor);
      m_PushScaleAnim->SetFinVal(scale * m_PushScaleFactor);
   }

   void ButtonAnimDecorator::SetColor(const math::vec4f& color)
   { 
      ButtonBaseDecorator::SetColor(
            math::vec4f(color.x, color.y, color.z, m_LostHoverColorAlpha));
      m_BaseColor = color;
      m_LostHoverColorAnim->SetFinVal(math::vec4f(color.x,
                                             color.y,
                                             color.z,
                                             m_LostHoverColorAlpha));

      m_HoverColorAnim->SetFinVal(math::vec4f(color.x,
                                             color.y,
                                             color.z,
                                             m_HoverColorAlpha));

      m_PushColorAnim->SetFinVal(math::vec4f(color.x,
                                             color.y,
                                             color.z,
                                             m_PushColorAlpha));
   }

   void ButtonAnimDecorator::OnLostHover()
   {
      m_AllAnimations.Pause();

      m_LostHoverScaleAnim->SetInitVal(GetScale());
      m_LostHoverScaleAnim->Reset();
      m_LostHoverScaleAnim->Play();

      m_LostHoverColorAnim->SetInitVal(GetColor());
      m_LostHoverColorAnim->Reset();
      m_LostHoverColorAnim->Play();
   }

   void ButtonAnimDecorator::OnHover()
   {
      m_AllAnimations.Pause();

      m_HoverScaleAnim->SetInitVal(GetScale());
      m_HoverScaleAnim->Reset();
      m_HoverScaleAnim->Play();

      m_HoverColorAnim->SetInitVal(GetColor());
      m_HoverColorAnim->Reset();
      m_HoverColorAnim->Play();
   }

   void ButtonAnimDecorator::OnPush()
   {
      m_AllAnimations.Pause();

      m_PushScaleAnim->SetInitVal(GetScale());
      m_PushScaleAnim->Reset();
      m_PushScaleAnim->Play();

      m_PushColorAnim->SetInitVal(GetColor());
      m_PushColorAnim->Reset();
      m_PushColorAnim->Play();
   }

   void ButtonAnimDecorator::ButtonListener::OnLostHover()
   {
      m_Master->OnLostHover();
   }

   void ButtonAnimDecorator::ButtonListener::OnHover()
   {
      m_Master->OnHover();
   }

   void ButtonAnimDecorator::ButtonListener::OnPush()
   {
      m_Master->OnPush();
   } 
}
