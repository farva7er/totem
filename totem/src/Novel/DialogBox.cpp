#include "DialogBox.h"

namespace totem
{
   DialogBox::DialogBox()
   {
      m_TextAnim = new StepAnimation(0, 0, 0.03f, false);
      m_Animator.Add(m_TextAnim);
   }

   void DialogBox::SetText(const Text& text)
   {
      TextBox::SetText(text);
      m_TextAnim->SetStart(0);
      m_TextAnim->SetEnd(text.GetLength());
      m_TextAnim->Reset();
      SetCharDisplayLimit(0);
      m_Animator.Sync(m_TextAnim);
   }

   void DialogBox::OnUpdate(float deltaTime)
   {
      m_Animator.OnUpdate(deltaTime);
      SetCharDisplayLimit(m_TextAnim->GetCurrIndex());
   }
}
