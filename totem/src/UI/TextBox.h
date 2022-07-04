#ifndef _TOTEM_TEXTBOX_H_
#define _TOTEM_TEXTBOX_H_

#include "Element.h"
#include "Internationalization/Text.h"

namespace totem
{
   class TextBox : public Element
   {
   public:
      TextBox(Ref<Font> font);
      TextBox(const TextBox& other) = default;

      ~TextBox() = default;

      TextBox& operator=(const TextBox& other) = default;

      //TODO
      //Text GetText() const { return m_TextWords.AssembleText(); }
      virtual void SetText(const Text& text);

      void SetLineSpacing(float spacing);
      void SetFont(Ref<Font> font);
      void SetFontSize(float size);
      void SetTextColor(const math::vec4f& color);
      void SetCharDisplayLimit(int charLimit);

      virtual void OnUpdate(float /*deltaTime*/) override {};
      virtual void OnEvent(Event& /*e*/) override {}
      virtual void Draw(Renderer* renderer) const override; 

   private:
      Text::TextVec m_TextWords;
      float m_LineSpacing;
      Ref<Font> m_Font;
      float m_FontSize;
      math::vec4f m_TextColor;
      int m_CharDisplayLimit;
   };
}

#endif
