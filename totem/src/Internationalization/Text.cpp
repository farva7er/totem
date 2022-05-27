#include "Text.h"
#include <cstring>

namespace totem
{
   Text::Text()
      : Text(nullptr, 0)
   {}

   Text::Text(const utf8_t* str, unsigned int sizeInBytes)
   {
      if(!str || !sizeInBytes)
      {
         m_Data = nullptr;
      }
      else
      {
         m_Data = new utf8_t[sizeInBytes];
         memcpy(m_Data, str, sizeInBytes);
      }
      m_AvailableBytes = sizeInBytes;
      m_OccupiedBytes = sizeInBytes;
   }

   Text::Text(unsigned int reserveBytes)
   {
      if(!reserveBytes)
      {
         m_Data = nullptr;
      }
      else
      {
         m_Data = new utf8_t[reserveBytes];
      }
      m_AvailableBytes = reserveBytes;
      m_OccupiedBytes = 0;
   }

   Text::~Text()
   {
      if(m_Data)
      {
         delete [] m_Data;
      }
   }

   Text::Text(const Text& other)
   {
      if(!other.m_OccupiedBytes)
      {
         m_Data = nullptr;
         m_AvailableBytes = 0;
         m_OccupiedBytes = 0;
      }
      else
      {
         m_Data = new utf8_t[other.m_OccupiedBytes];
         memcpy(m_Data, other.m_Data, other.m_OccupiedBytes);
         m_AvailableBytes = other.m_OccupiedBytes;
         m_OccupiedBytes = other.m_OccupiedBytes;
      }
   }

   Text& Text::operator=(const Text& other)
   {
      if(this == &other)
         return *this;

      if(!other.m_OccupiedBytes)
      {
         // Save allocated memory for later use
         m_OccupiedBytes = 0;
         return *this;
      }

      // Does it have enough space already ?
      if(m_AvailableBytes >= other.m_OccupiedBytes)
      {
         memcpy(m_Data, other.m_Data, other.m_OccupiedBytes);
         m_OccupiedBytes = other.m_OccupiedBytes;
      }
      else
      {
         // It is safe to delete before copy in this case
         // Two different Text objects cannot contain intersecting memory
         if(m_Data)
         {
            delete [] m_Data;
         }

         m_Data = new utf8_t[other.m_OccupiedBytes];
         memcpy(m_Data, other.m_Data, other.m_OccupiedBytes);
         m_AvailableBytes = other.m_OccupiedBytes;
         m_OccupiedBytes = other.m_OccupiedBytes;
      }

      return *this;
   }

   Text& Text::operator+=(char c)
   {
      if(m_OccupiedBytes == 0)
      {
         m_AvailableBytes = 1;
         m_Data = new utf8_t[1];
      }
      // if there are no empty bytes
      else if(m_OccupiedBytes == m_AvailableBytes)
      {
         utf8_t* newData = new utf8_t[2 * m_AvailableBytes];
         m_AvailableBytes *= 2;
         memcpy(newData, m_Data, m_OccupiedBytes);
         delete [] m_Data;
         m_Data = newData;
      }

      m_Data[m_OccupiedBytes] = c;
      m_OccupiedBytes++;

      return *this;
   }


   utf8_t* Text::GetRawData()
   {
      return m_Data;
   }

   const utf8_t* Text::GetRawData() const
   {
      return m_Data;
   }

   unsigned int Text::GetRawSize() const
   {
      return m_OccupiedBytes;
   }

   int Text::GetLength() const
   {
      int res = 0;
      Iterator iter(*this);
      while(!iter.HasEnded())
      {
         res++;
         iter.Next();
      }
      return res;
   }

   utf8_t Text::operator[](unsigned i) const
   {
      return m_Data[i];
   }

   Text::Iterator::Iterator(const Text& text)
      : m_Text(&text),
      m_StartOffset(0), m_Length(-1),
      m_CurrIndex(0), m_CurrByte(0),
      m_CurrCodepoint(0), m_HasEnded(false)
   {
      ExtractCodepoint(GetNumBytes());
      m_CurrIndex = 0;
   }

   Text::Iterator::Iterator(ConstSubText subText)
      : m_Text(&subText.GetMaster()),
      m_StartOffset(subText.GetStartPos()),
      m_Length(-1),
      m_CurrIndex(0), m_CurrByte(0),
      m_CurrCodepoint(0), m_HasEnded(false)
   {
      ExtractCodepoint(GetNumBytes());
      for(int i = 0; i < subText.GetStartPos(); i++)
      {
         Next();
      }
      m_Length = subText.GetLength();
      m_CurrIndex = 0;
   }

   void Text::Iterator::ExtractCodepoint(unsigned int numBytes) 
   {
      if(numBytes && ValidateConBytes(numBytes) && !IsEndReached())
      {
         m_CurrCodepoint = ExtractValue(numBytes);
         m_CurrIndex++;
      }
      else
      {
         m_HasEnded = true;
         m_CurrCodepoint = 0;
      }
   }

   bool Text::Iterator::IsEndReached() const
   {
      if(m_Length == -1)
         return false;
      return m_CurrIndex >= m_Length - 1;
   }

   void Text::Iterator::Next()
   {
      if(HasEnded())
         return;

      m_CurrByte += GetNumBytes();
      ExtractCodepoint(GetNumBytes());
   }

   unicode_t Text::Iterator::Get() const
   {
      return m_CurrCodepoint;
   }

   int Text::Iterator::GetIndex() const
   {
      return m_CurrIndex;
   }

   bool Text::Iterator::HasEnded() const
   {
      return m_HasEnded;
   }

   unsigned int Text::Iterator::GetNumBytes() const
   {
      if(m_CurrByte >= m_Text->m_OccupiedBytes)
         return 0;

      utf8_t b = (*m_Text)[m_CurrByte];
      int n;
      if((b & 0x80) == 0)
      {
         n = 1;
      }
      else if ((b & 0xE0) == 0xC0)
      {
         n = 2;
      }
      else if ((b & 0xF0) == 0xE0)
      {
         n = 3;
      }
      else if ((b & 0xF8) == 0xF0) 
      {
         n = 4;
      }
      else
      {
         // Invalid UTF-8 data
         n = 0;
      }
      return n;
   }

   bool Text::Iterator::ValidateConBytes(unsigned int numBytes) const
   {
      unsigned int remainingBytes = m_Text->m_OccupiedBytes - m_CurrByte;
      if(remainingBytes < numBytes)
      {
         // Expected numBytes bytes, but there are no that many bytes
         return false;
      }

      for(unsigned int i = 1; i < numBytes; i++)
      {
         utf8_t conByte = (*m_Text)[m_CurrByte + i];
         if((conByte & 0xC0) != 0x80)
         {
            // Expected continuation byte, but expectation failed
            return false;
         }
      }
      return true;
   }


   unicode_t Text::Iterator::ExtractValue(unsigned int numBytes) const
   {
      utf8_t b = (*m_Text)[m_CurrByte];
      unicode_t codepoint = 0;
      if(numBytes == 1)
      {
         codepoint = b & 0x7F;
      }
      else if (numBytes == 2)
      {
         codepoint = b & 0x1F;
      }
      else if (numBytes == 3)
      {
         codepoint = b & 0x0F;
      }
      else if (numBytes == 4) 
      {
         codepoint = b & 0x07;
      }
      else
      {
         // Invalid UTF-8 data
         return 0;
      }

      for(unsigned int i = 1; i < numBytes; i++)
      {
         utf8_t cb = (*m_Text)[m_CurrByte + i];
         codepoint = codepoint << 6;
         codepoint |= (cb & 0x3F);
      }
      return codepoint;
   }


   bool Text::IsBlank(unicode_t c)
   {
      // SP, HTAB, CR, LF
      return c == 0x20 || c == 0x09 || c == 0x0D || c == 0x0A;
   }
 
   Text::ConstSubTextVec Text::GetWords() const
   {
      ConstSubTextVec res;
      Iterator iter(*this);
      unsigned int wordStartPos = 0, wordLen = 0;

      while(!iter.HasEnded())
      {
         // Skip all Blank Characters
         while(!iter.HasEnded())
         {
            if(!IsBlank(iter.Get()))
               break;
            iter.Next();
         }
         // If nothing is left then return result (may be empty)
         if(iter.HasEnded())
            return res;

         wordStartPos = iter.GetIndex();
         wordLen = 0;
         while(!iter.HasEnded())
         {
            unicode_t c = iter.Get();
            if(!IsBlank(c))
            {
               wordLen++;
            }
            else
            {
               res.Add(ConstSubText(*this, wordStartPos, wordLen));
               break;
            }

            iter.Next();
            if(iter.HasEnded())
            {
               res.Add(ConstSubText(*this, wordStartPos, wordLen));
               break;
            }
         }
      }
      return res;
   }

   Text::SubTextVec Text::GetWords()
   {
      ConstSubTextVec constRes = 
         (static_cast<const Text*>(this))->GetWords();
      SubTextVec res;
      for(int i = 0; i < constRes.GetCount(); i++)
      {
         res[i] = SubText(*this, constRes[i].GetStartPos(),
                                 constRes[i].GetLength());
      }
      return res;
   }
}
