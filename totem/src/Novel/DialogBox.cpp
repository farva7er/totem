#include "DialogBox.h"

namespace totem
{
   DialogBox::DialogBox(Ref<Font> font)
      : TextBox(font)
   {
      m_TextAnim = new SeqAnimation(0, 0, 0.03f, false);
   }

   DialogBox::DialogBox(const DialogBox& other)
      : TextBox(other)
   {
      m_TextAnim = new SeqAnimation(*other.m_TextAnim);
   }

   DialogBox::~DialogBox()
   {
      delete m_TextAnim;
   }

   DialogBox& DialogBox::operator=(const DialogBox& other)
   {
      m_TextAnim = new SeqAnimation(*other.m_TextAnim);
      return *this;
   }

   void DialogBox::SetText(const Text& text)
   {
      TextBox::SetText(text);
      m_TextAnim->SetStart(0);
      m_TextAnim->SetEnd(text.GetLength());
      m_TextAnim->Reset();
      m_Animator.Sync(m_TextAnim);
      SetCharDisplayLimit(0);
   }

   void DialogBox::OnUpdate(float deltaTime)
   {
      m_Animator.OnUpdate(deltaTime);
      if(!m_TextAnim->IsPaused())
         SetCharDisplayLimit(m_TextAnim->GetCurrIndex());
   }

   bool DialogBox::IsAnimationPlaying() const
   {
      return !m_TextAnim->IsPaused();
   }

   void DialogBox::SkipAnimation()
   {
      SetCharDisplayLimit(-1);
      m_TextAnim->Pause();
   }
}
