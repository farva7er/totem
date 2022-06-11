#ifndef _TOTEM_NOVEL_DIALOG_BOX_H_
#define _TOTEM_NOVEL_DIALOG_BOX_H_

#include "UI/TextBox.h"
#include "Animations/BasicAnimations/StepAnimation.h"
#include "Animations/Animator.h"

namespace totem
{
   class DialogBox : public TextBox
   {
   public:
      DialogBox();
      virtual void SetText(const Text& text) override;
      virtual void OnUpdate(float deltaTime) override;
   private:
      StepAnimation* m_TextAnim;
      Animator m_Animator;
   };
}

#endif
