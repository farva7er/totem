#ifndef _TOTEM_NOVEL_DIALOG_BOX_H_
#define _TOTEM_NOVEL_DIALOG_BOX_H_

#include "UI/TextBox.h"
#include "Animations/BasicAnimations/SeqAnimation.h"
#include "Animations/Animator.h"
#include "UI/Clickable.h"

namespace totem
{
   class DialogOption : public Clickable
   {
      public:
         DialogOption(Ref<Font> defaultFont, int optionIndex);

         void SetText(const Text& text);

         void SetFontSize(float fontSize);
         void SetTextColor(const math::vec4f& color);
         void SetFont(Ref<Font> font);

         virtual void OnClick() override;

         virtual void OnUpdate(float /*deltaTime*/) override {}
         virtual void Draw(Renderer* renderer) const override;

      private:
         int m_OptionIndex;
         float m_FontSize;
         math::vec4f m_TextColor;
         Ref<Font> m_Font;
         Text m_Text;
   };

   class DialogBox : public TextBox
   {
      public:
         DialogBox();
         DialogBox(const DialogBox& other);

         ~DialogBox();

         DialogBox& operator=(const DialogBox& other);

         virtual void OnEvent(Event& e) override;
         virtual void OnUpdate(float deltaTime) override;
         virtual void Draw(Renderer* renderer) const override;

         virtual void SetText(const Text& text) override;
         void SetCharacterName(const Text& name);
         void SetCharacterNameColor(const math::vec4f& color);
         bool IsAnimationPlaying() const;
         void SkipAnimation();

         void ClearDialogOptions();
         void SetDialogOptionText(int optionIndex, const Text& text);
         int GetSelectedDialogOption() const;
         void SetSelectedDialogOption(int selectedIndex);
         
      private:
         math::vec4f m_CharacterNameColor;
         Text m_CharacterName;
         SeqAnimation m_TextAnim;
         Animator m_Animator;

         enum { dialogOptionsCount = 4 };
         DialogOption* m_DialogOptions[dialogOptionsCount];
         int m_SelectedDialogOption;
   };
}

#endif
