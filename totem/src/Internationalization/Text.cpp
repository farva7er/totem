#include "Text.h"
#include <cstring>

namespace totem
{
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

   utf8_t Text::operator[](unsigned i) const
   {
      return m_Data[i];
   }


   Text::Iterator Text::GetIterator() const
   {
      return Iterator(this);
   }

   Text::Iterator::Iterator(const Text* master)
      : m_Master(master),
      m_CurrByte(0),
      m_CurrCodepoint(0),
      m_HasEnded(false)
   {
      ExtractCodepoint(GetNumBytes());
   }

   void Text::Iterator::ExtractCodepoint(unsigned int numBytes) 
   {
      if(numBytes && ValidateConBytes(numBytes))
      {
         m_CurrCodepoint = ExtractValue(numBytes);
      }
      else
      {
         m_HasEnded = true;
         m_CurrCodepoint = 0;
      }
   }

   void Text::Iterator::Next()
   {
      if(HasEnded())
         return;
      int n = GetNumBytes();
      m_CurrByte += n;
      ExtractCodepoint(n);
   }

   unicode_t Text::Iterator::Get() const
   {
      return m_CurrCodepoint;
   }

   bool Text::Iterator::HasEnded() const
   {
      return m_HasEnded;
   }

   unsigned int Text::Iterator::GetNumBytes() const
   {
      if(m_CurrByte >= m_Master->m_OccupiedBytes)
         return 0;

      utf8_t b = (*m_Master)[m_CurrByte];
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
      unsigned int remainingBytes = m_Master->m_OccupiedBytes - m_CurrByte;
      if(remainingBytes < numBytes)
      {
         // Expected numBytes bytes, but there are no that many bytes
         return false;
      }

      for(unsigned int i = 1; i < numBytes; i++)
      {
         utf8_t conByte = (*m_Master)[m_CurrByte + i];
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
      utf8_t b = (*m_Master)[m_CurrByte];
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
         utf8_t cb = (*m_Master)[m_CurrByte + i];
         codepoint = codepoint << 6;
         codepoint |= (cb & 0x3F);
      }
      return codepoint;
   }
}
