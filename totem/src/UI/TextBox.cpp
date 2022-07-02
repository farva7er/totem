#include "TextBox.h"
#include "Log.h"
#include "Renderer/RendererPrimitives/Rect.h"

namespace totem
{
   TextBox::TextBox(Ref<Font> font)
      : m_LineSpacing(1), m_Font(font), m_FontSize(1),
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

   void TextBox::SetLineSpacing(float spacing)
   {
      m_LineSpacing = spacing;
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

      // Draw TextBox background rect.
      Rect rect;
      rect.SetPos(m_Pos)
         .SetScale(m_Scale)
         .SetColor(grayColor);

      renderer->DrawRect(rect);

      // Calculate line spacing.
      float lineSpacing = 2 * renderer->GetEM() * m_FontSize * m_LineSpacing;

      // Initial positions.
      float xPos = m_Pos.x - m_Scale.x;
      float yPos = m_Pos.y + m_Scale.y - lineSpacing;

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
         if(xPos + wordBBox.x + spaceAdvance > m_Pos.x + m_Scale.x)
         {
            // Go to next line.
            yPos -= lineSpacing;
            xPos = m_Pos.x - m_Scale.x;
         }

         // If this line is out of the text box.
         if(yPos < m_Pos.y - m_Scale.y)
         {
            // No more text can be displayed.
            return;
         }


         // DETERMINE IF FULL WORD CAN BE DISPLAYED.

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
