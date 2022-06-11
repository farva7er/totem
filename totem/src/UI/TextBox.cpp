#include "TextBox.h"
#include "Log.h"
#include "Renderer/RendererPrimitives/Rect.h"

namespace totem
{
   TextBox::TextBox()
      : m_LineSpacing(1), m_FontSize(1),
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

      Rect rect = Rect::Builder()
                        .SetPos(m_Pos)
                        .SetScale(m_Scale)
                        .SetColor(grayColor)
                        .Construct();

      renderer->DrawRect(rect);

      float lineSpacing = 2 * renderer->GetCharBaseScale().y 
                        * m_FontSize * m_LineSpacing;

      float xPos = m_Pos.x - m_Scale.x;
      float yPos = m_Pos.y + m_Scale.y - lineSpacing;

      unicode_t space = 32;
      float spaceAdvance = renderer->GetCharAdvance(space, m_FontSize);

      int charsDisplayed = 0;
      for(int i = 0; i < m_TextWords.GetCount(); i++)
      {
         math::vec2f wordBBox = renderer->CalcBBox(m_TextWords[i], m_FontSize);
         if(xPos + wordBBox.x + spaceAdvance > m_Pos.x + m_Scale.x)
         {
            yPos -= lineSpacing;
            xPos = m_Pos.x - m_Scale.x;
         }
         if(yPos < m_Pos.y - m_Scale.y)
         {
            return;
         }

         // Check the limit is set
         if(m_CharDisplayLimit < 0)
         {
            // If not than just draw everything
            renderer->DrawText(m_TextWords[i],
                           { xPos, yPos }, m_FontSize, m_TextColor);
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
                           { xPos, yPos }, m_FontSize, m_TextColor);
               // Since limit is already exceeded nothing more needs
               // to be displayed
               return;
            }
            else
            {
               // Limit is not exceeded, just render full word
               renderer->DrawText(m_TextWords[i],
                           { xPos, yPos }, m_FontSize, m_TextColor);
               charsDisplayed += wordLen;
            }
         }
         // We get here only if full word is rendered
         xPos += wordBBox.x + spaceAdvance; 
      } 
   } 
}
