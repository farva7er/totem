#ifndef _TOTEM_NOVEL_DIALOG_BOX_H_
#define _TOTEM_NOVEL_DIALOG_BOX_H_

#include "UI/TextBox.h"
#include "Animations/BasicAnimations/SeqAnimation.h"
#include "Animations/Animator.h"

namespace totem
{
//   class DialogOption : public Clickable
//   {
//      public:
//         const Text& GetText() const;
//         void SetText(const Text& text);
//
//         virtual void OnClick() override;
//
//         virtual void OnUpdate(float deltaTime) override {}
//         virtual void Draw(Renderer* renderer) const override;
//
//      private:
//         TextVec m_TextWords;
//   };
//
   class DialogBox : public TextBox
   {
      public:
         DialogBox();
         DialogBox(const DialogBox& other);

         ~DialogBox();

         DialogBox& operator=(const DialogBox& other);

         virtual void OnUpdate(float deltaTime) override;
         virtual void Draw(Renderer* renderer) const override;

         virtual void SetText(const Text& text) override;
         void SetCharacterName(const Text& name);
         void SetCharacterNameColor(const math::vec4f& color);
         bool IsAnimationPlaying() const;
         void SkipAnimation();
         
      private:
         math::vec4f m_CharacterNameColor;
         Text m_CharacterName;
         SeqAnimation* m_TextAnim;
         Animator m_Animator;

         //DialogOption m_DialogOptions[4];
   };
}

#endif
