#include <string.h>

#include "Button.h"

namespace totem
{
   ////////////////////////////////////////////////////////////////////
   /////         BoxButton       //////////////////////////////////////
   ////////////////////////////////////////////////////////////////////
   BoxButton::BoxButton()
      : m_Color(math::vec4f(0, 0, 0, 1))
   {}

   BoxButton::~BoxButton()
   {}

   BoxButton::BoxButton(const BoxButton& other)
      : InteractiveElementImpl(other)
   {
      m_Text = other.m_Text;
   }

   BoxButton& BoxButton::operator=(const BoxButton& other)
   {
      InteractiveElementImpl::operator=(other);
      m_Text = other.m_Text;
      return *this;
   }

   void BoxButton::Draw(Renderer* renderer) const
   {
      const math::vec4f grayColor{0.5f, 0.5f, 0.5f, 0.5f};
      const math::vec4f& color = IsActive() ? m_Color : grayColor;

      Rect rect = Rect::Builder()
                        .SetPos(m_Pos)
                        .SetScale(m_Scale)
                        .SetColor(color)
                        .Construct();

      renderer->DrawRect(rect);
      if(!m_Text.IsEmpty())
      {
         
         renderer->DrawAlignedText(
                     m_Text, m_Pos, m_Scale, m_Scale.y * 0.7f,
                     math::vec4f(1, 1, 1, 1),
                     TextAlign::VCenter | TextAlign::HCenter
                     );
      }
   }

   void BoxButton::SetText(const Text& text)
   {
      m_Text = text;
   }

//////////////////////////////////////////////////////////////
////////         ButtonBaseDecorator      ////////////////////
//////////////////////////////////////////////////////////////

   void ButtonBaseDecorator::Draw(Renderer* renderer) const
   {
      m_Wrapee->Draw(renderer);
   }

   void ButtonBaseDecorator::AddListener(IIEListener* listener)
   {
      m_Wrapee->AddListener(listener);
   }

//////////////////////////////////////////////////////////////
////////       ButtonScaleDecorator     //////////////////////
//////////////////////////////////////////////////////////////

   ButtonScaleDecorator::ButtonScaleDecorator(IButton* button)
      : ButtonBaseDecorator(button)
   {
      m_BaseScale = button->GetScale();

      m_LostHoverAnimDuration = 0.3f;
      m_HoverAnimDuration = 0.2f;
      m_PushAnimDuration = 0.1f;

      m_HoverScaleFactor = 1.1f;
      m_PushScaleFactor = 1.05f;

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

      m_AllAnimations.Add(m_LostHoverScaleAnim);
      m_AllAnimations.Add(m_HoverScaleAnim);
      m_AllAnimations.Add(m_PushScaleAnim);

      m_Animator.Add(m_AllAnimations);

      button->AddListener(new ButtonListener(this));
   }

   void ButtonScaleDecorator::OnUpdate(float deltaTime)
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

      m_Wrapee->OnUpdate(deltaTime);
   }

   void ButtonScaleDecorator::SetScale(const math::vec2f& scale)
   {
      ButtonBaseDecorator::SetScale(scale);
      m_BaseScale = scale;
      m_LostHoverScaleAnim->SetFinVal(scale);
      m_HoverScaleAnim->SetFinVal(scale * m_HoverScaleFactor);
      m_PushScaleAnim->SetFinVal(scale * m_PushScaleFactor);
   }
 
   void ButtonScaleDecorator::OnLostHover()
   {
      m_AllAnimations.Pause();

      m_LostHoverScaleAnim->SetInitVal(GetScale());
      m_LostHoverScaleAnim->Reset();
      m_LostHoverScaleAnim->Play();
   }

   void ButtonScaleDecorator::OnHover()
   {
      m_AllAnimations.Pause();

      m_HoverScaleAnim->SetInitVal(GetScale());
      m_HoverScaleAnim->Reset();
      m_HoverScaleAnim->Play();
   }

   void ButtonScaleDecorator::OnPush()
   {
      m_AllAnimations.Pause();

      m_PushScaleAnim->SetInitVal(GetScale());
      m_PushScaleAnim->Reset();
      m_PushScaleAnim->Play();
   }

   void ButtonScaleDecorator::ButtonListener::OnLostHover()
   {
      m_Master->OnLostHover();
   }

   void ButtonScaleDecorator::ButtonListener::OnHover()
   {
      m_Master->OnHover();
   }

   void ButtonScaleDecorator::ButtonListener::OnPush()
   {
      m_Master->OnPush();
   }

   IIEListener* ButtonScaleDecorator::ButtonListener::Clone() const
   {
      return new ButtonScaleDecorator::ButtonListener(*this);
   }

//////////////////////////////////////////////////////////////
////////       ButtonColorDecorator     //////////////////////
//////////////////////////////////////////////////////////////

   ButtonColorDecorator::ButtonColorDecorator(IButton* button)
      : ButtonBaseDecorator(button)
   {
      m_BaseColor = button->GetColor();


      m_LostHoverAnimDuration = 0.3f;
      m_HoverAnimDuration = 0.2f;
      m_PushAnimDuration = 0.1f;

      m_LostHoverColorAlpha = 0.7f;
      m_HoverColorAlpha = 0.9f;
      m_PushColorAlpha = 1.0f;

      
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

      m_AllAnimations.Add(m_LostHoverColorAnim);
      m_AllAnimations.Add(m_HoverColorAnim);
      m_AllAnimations.Add(m_PushColorAnim);

      m_Animator.Add(m_AllAnimations);

      button->AddListener(new ButtonListener(this));
   }

   void ButtonColorDecorator::OnUpdate(float deltaTime)
   {
      m_Animator.OnUpdate(deltaTime);

      math::vec4f color;

      m_LostHoverColorAnim->ApplyVal(color);
      m_HoverColorAnim->ApplyVal(color);
      m_PushColorAnim->ApplyVal(color);

      if(!m_LostHoverColorAnim->IsPaused() || !m_HoverColorAnim->IsPaused() ||
         !m_PushColorAnim->IsPaused())
      {
         ButtonBaseDecorator::SetColor(color);
      }

      m_Wrapee->OnUpdate(deltaTime);
   }

   void ButtonColorDecorator::SetColor(const math::vec4f& color)
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

   void ButtonColorDecorator::OnLostHover()
   {
      m_AllAnimations.Pause();

      m_LostHoverColorAnim->SetInitVal(GetColor());
      m_LostHoverColorAnim->Reset();
      m_LostHoverColorAnim->Play();
   }

   void ButtonColorDecorator::OnHover()
   {
      m_AllAnimations.Pause();

      m_HoverColorAnim->SetInitVal(GetColor());
      m_HoverColorAnim->Reset();
      m_HoverColorAnim->Play();
   }

   void ButtonColorDecorator::OnPush()
   {
      m_AllAnimations.Pause();

      m_PushColorAnim->SetInitVal(GetColor());
      m_PushColorAnim->Reset();
      m_PushColorAnim->Play();
   }

   void ButtonColorDecorator::ButtonListener::OnLostHover()
   {
      m_Master->OnLostHover();
   }

   void ButtonColorDecorator::ButtonListener::OnHover()
   {
      m_Master->OnHover();
   }

   void ButtonColorDecorator::ButtonListener::OnPush()
   {
      m_Master->OnPush();
   }

   IIEListener* ButtonColorDecorator::ButtonListener::Clone() const
   {
      return new ButtonColorDecorator::ButtonListener(*this);
   }
}
