#include "TextBox.h"
#include "Log.h"
#include "Rendering/Primitives/Rect.h"

namespace totem
{
   TextBox::TextBox(Ref<Font> font)
      : m_Padding(0, 0, 0, 0), m_LineSpacing(1), m_Font(font), m_FontSize(1),
      m_TextColor{0, 0, 0, 1}, m_CharDisplayLimit(-1)
   {}

   void TextBox::SetText(const Text& text)
   {
      Text::ConstSubTextVec words = text.GetWords();
      m_TextWords.Clear();
      for(int i = 0; i < words.GetCount(); i++)
      {
         m_TextWords.Add(words[i].GetText());
      }
   }

   void TextBox::SetPadding(const math::vec4f& padding)
   {
      m_Padding = padding;
   }

   void TextBox::SetLineSpacing(float spacing)
   {
      m_LineSpacing = spacing;
   }

   Ref<Font> TextBox::GetFont() const
   {
      return m_Font;
   }

   void TextBox::SetFont(Ref<Font> font)
   {
      m_Font = font;
   }

   float TextBox::GetFontSize() const
   {
      return m_FontSize;
   }

   void TextBox::SetFontSize(float size)
   {
      m_FontSize = size;
   }

   void TextBox::SetTextColor(const math::vec4f& color)
   {
      m_TextColor = color;
   }

   void TextBox::SetCharDisplayLimit(int charLimit)
   {
      m_CharDisplayLimit = charLimit;
   }

   void TextBox::Draw(Renderer* renderer) const
   {
      const math::vec4f grayColor{0, 0, 0, 0.7f};
      const math::vec2f& pos = GetPos();
      const math::vec2f& scale = GetScale();

      // Draw TextBox background rect.
      Rect rect;
      rect.SetPos(pos)
         .SetScale(scale)
         .SetColor(grayColor);

      renderer->DrawRect(rect);

      // Calculate line spacing.
      float lineSpacing = 2 * renderer->GetEM() * m_FontSize * m_LineSpacing;

      // Initial positions.
      float xPos = pos.x - scale.x + m_Padding.w;
      float yPos = pos.y + scale.y - lineSpacing - m_Padding.x;

      // Get space advance in canvas coords.
      unicode_t space = 32;
      float spaceAdvance = m_Font->GetAdvance(space) * 2 * renderer->GetEM() *
                           m_FontSize;

      int charsDisplayed = 0;
      for(int i = 0; i < m_TextWords.GetCount(); i++)
      {
         // WORD POSITION CALCULATIONS.

         // Calculate current word bounding box width and height.
         math::vec2f wordBBox = 2 * renderer->CalcBBox(m_TextWords[i],
                                                   m_FontSize, *m_Font);

         // If current word does not fit on current line.
         if(xPos + wordBBox.x + spaceAdvance > pos.x + scale.x - m_Padding.y)
         {
            // Go to next line.
            yPos -= lineSpacing;
            xPos = pos.x - scale.x + m_Padding.w;
         }

         // If this line is out of the text box.
         if(yPos < pos.y - scale.y + m_Padding.z)
         {
            // No more text can be displayed.
            return;
         }


         // DETERMINE IF FULL WORD CAN BE DISPLAYED.
         //
         // Check if limit is set
         if(m_CharDisplayLimit < 0)
         {
            // If not then just draw all available text.
            renderer->DrawText(m_TextWords[i],
                           { xPos, yPos }, *m_Font, m_FontSize, m_TextColor);
         }
         else
         {
            // Trying to display one full word with a space
            int wordLen = m_TextWords[i].GetLength() + 1; // +1 for space
            // Will we excess the limit if full word with a space is displayed?
            int excessCharNum = (charsDisplayed + wordLen) 
                                       - m_CharDisplayLimit;
            if(excessCharNum > 0)
            {
               // Shorten the word
               Text::ConstSubText shortWord(m_TextWords[i],
                                       0, wordLen - excessCharNum);
               renderer->DrawText(shortWord.GetText(),
                           { xPos, yPos }, *m_Font, m_FontSize, m_TextColor);
               // Since limit is already exceeded nothing more needs
               // to be displayed
               return;
            }
            else
            {
               // Limit is not exceeded, just render full word
               renderer->DrawText(m_TextWords[i],
                           { xPos, yPos }, *m_Font, m_FontSize, m_TextColor);
               charsDisplayed += wordLen;
            }
         }
         // We get here only if full word is rendered
         xPos += wordBBox.x + spaceAdvance; 
      } 
   } 
}
