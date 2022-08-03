#include "DialogBox.h"
#include "NovelApp.h"

namespace totem
{
   DialogOption::DialogOption(Ref<Font> defaultFont, int optionIndex)
      : m_OptionIndex(optionIndex), m_FontSize(0.5f),
      m_TextColor(1, 1, 1, 1), m_Font(defaultFont)
   {}

   void DialogOption::SetText(const Text& text)
   {
      m_Text = text;
   }

   void DialogOption::SetFontSize(float fontSize)
   {
      m_FontSize = fontSize;
   }

   void DialogOption::SetTextColor(const math::vec4f& color)
   {
      m_TextColor = color;
   }

   void DialogOption::SetFont(Ref<Font> font)
   {
      m_Font = font;
   }

   void DialogOption::OnClick()
   { 
      NovelApp::GetInstance()->ChooseDialogOption(m_OptionIndex);
   }

   void DialogOption::Draw(Renderer* renderer) const
   {
      if(!IsActive())
         return;

      Rect rect;
      rect
         .SetPos(GetPos())
         .SetScale(GetScale())
         .SetColor({ 0.1f, 0.1f, 0.1f, 0.7f });
      renderer->DrawRect(rect);

      math::vec2f bbox = renderer->CalcBBox(m_Text, m_FontSize, *m_Font);
      math::vec2f textPos = { GetPos().x - GetScale().x + 0.5f,
                              GetPos().y - bbox.y };



      renderer->DrawText(m_Text, textPos, *m_Font, m_FontSize, m_TextColor);
   }

   DialogBox::DialogBox()
      : TextBox(NovelApp::GetResourceManager()
            ->Get<Font>("resources/fonts/OpenSans-Regular.ttf")),
      m_TextAnim(0, 0, 0.03f, false), m_SelectedDialogOption(0) 
   {
      SetScale({10, 2});
      SetPos({0, -7});
      SetTextColor({ 0.9f, 0.9f, 0.2f, 1.0f });
      SetFontSize(0.6f);
      SetLineSpacing(1.5f);
      SetPadding({0.4f, 1, 0, 1});

      for(int i = 0; i < dialogOptionsCount; i++)
         m_DialogOptions[i] = new DialogOption(GetFont(), i);

      for(int i = 0; i < dialogOptionsCount; i++)
      {
         m_DialogOptions[i]->SetPos({ 0, -5.6f - i * 1.1f });
         m_DialogOptions[i]->SetScale({ 9.0f, 0.4f });
      }
   }

   DialogBox::DialogBox(const DialogBox& other)
      : TextBox(other), m_CharacterNameColor(other.m_CharacterNameColor),
      m_CharacterName(other.m_CharacterName), m_TextAnim(other.m_TextAnim),
      m_SelectedDialogOption(other.m_SelectedDialogOption)
   {
      for(int i = 0; i < dialogOptionsCount; i++)
      {
         m_DialogOptions[i] = new DialogOption(*other.m_DialogOptions[i]);
      }
   }

   DialogBox::~DialogBox()
   {
      for(int i = 0; i < dialogOptionsCount; i++)
         delete m_DialogOptions[i];
   }

   DialogBox& DialogBox::operator=(const DialogBox& other)
   {
      m_CharacterNameColor = other.m_CharacterNameColor;
      m_CharacterName = other.m_CharacterName;
      m_TextAnim = other.m_TextAnim;
      m_Animator.Clear();
      for(int i = 0; i < dialogOptionsCount; i++)
      {
         if(m_DialogOptions[i])
            delete m_DialogOptions[i];
         m_DialogOptions[i] = new DialogOption(*other.m_DialogOptions[i]);
      }
      m_SelectedDialogOption = other.m_SelectedDialogOption;
      return *this;
   }

   void DialogBox::SetText(const Text& text)
   {
      TextBox::SetText(text);
      m_TextAnim.SetStart(0);
      m_TextAnim.SetEnd(text.GetLength());
      m_TextAnim.Reset();
      m_Animator.Sync(&m_TextAnim);
      SetCharDisplayLimit(0);
   }

   void DialogBox::SetCharacterName(const Text& text)
   {
      m_CharacterName = text;
   }

   void DialogBox::SetCharacterNameColor(const math::vec4f& color)
   {
      m_CharacterNameColor = color;
   }


   void DialogBox::OnEvent(Event& e)
   {
      for(int i = 0; i < dialogOptionsCount; i++)
      {
         m_DialogOptions[i]->OnEvent(e);
      }
   }

   void DialogBox::OnUpdate(float deltaTime)
   {
      m_Animator.OnUpdate(deltaTime);
      if(!m_TextAnim.IsPaused())
         SetCharDisplayLimit(m_TextAnim.GetCurrIndex());
      for(int i = 0; i < dialogOptionsCount; i++)
      {
         m_DialogOptions[i]->OnUpdate(deltaTime);
      }
   }

   void DialogBox::Draw(Renderer* renderer) const
   {
      TextBox::Draw(renderer);

      const math::vec2f pos = GetPos();
      const math::vec2f scale = GetScale();
     
      math::vec2f textPos{ pos.x - scale.x + 1.0f, pos.y + scale.y - 0.5f};
      renderer->DrawText(m_CharacterName, textPos,
            *GetFont(), GetFontSize() * 1.1f, m_CharacterNameColor);

      for(int i = 0; i < dialogOptionsCount; i++)
      {
         m_DialogOptions[i]->Draw(renderer);
      }
   }

   bool DialogBox::IsAnimationPlaying() const
   {
      return !m_TextAnim.IsPaused();
   }

   void DialogBox::SkipAnimation()
   {
      SetCharDisplayLimit(-1);
      m_TextAnim.Pause();
   }

   void DialogBox::ClearDialogOptions()
   {
      for(int i = 0; i < dialogOptionsCount; i++)
      {
         m_DialogOptions[i]->SetText("");
         m_DialogOptions[i]->SetActive(false);
      }
   }

   void DialogBox::SetDialogOptionText(int optionIndex, const Text& text)
   {
      if(optionIndex < 0 || optionIndex >= dialogOptionsCount)
         return;
      m_DialogOptions[optionIndex]->SetText(text);
      m_DialogOptions[optionIndex]->SetActive(true);
   }

   int DialogBox::GetSelectedDialogOption() const
   {
      return m_SelectedDialogOption;
   }

   void DialogBox::SetSelectedDialogOption(int selectedIndex)
   {
      m_SelectedDialogOption = selectedIndex; 
   }
}
