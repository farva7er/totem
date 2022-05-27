#ifndef _INTERNATIONALIZATION_TEXT_H_
#define _INTERNATIONALIZATION_TEXT_H_

namespace totem
{

   typedef unsigned char utf8_t;
   typedef unsigned int unicode_t;

   class Text
   {
   public:
      Text();
      // Construct Text from a given utf-8 buffer
      Text(const utf8_t* str, unsigned int sizeInBytes);

      // An optimisation hint, reserve some bytes upfront
      Text(unsigned int reserveBytes);

      ~Text();
      Text(const Text& other);
      Text& operator=(const Text& other);

      Text& operator+=(char c);
      utf8_t* GetRawData();
      const utf8_t* GetRawData() const;
      unsigned int GetRawSize() const;

      int GetLength() const;

      template <typename TextType>
      class SubTextRW;

      template <typename TextType>
      class SubTextR
      {
      public:
         SubTextR() : m_Text(nullptr), m_StartPos(0), m_Length(-1) {}
         // positive startPos means offset from the beggining of the text,
         // negative startPos means offset from the end of the text(master)
         // -1 length means all available text starting from startPos
         SubTextR(TextType& master, int startPos = 0, int length = -1);
         SubTextR(const SubTextR& other) = default;
         SubTextR(SubTextRW<Text> subText);
         ~SubTextR() = default;

         SubTextR& operator=(const SubTextR& other) = default;

         bool operator==(const SubTextR& other) const;

         int GetStartPos() const { return m_StartPos; }
         int GetLength() const { return m_Length; }
         int GetByteLength() const;

         TextType& GetMaster() const { return *m_Text; }
 
      private:
         TextType* m_Text;
         int m_StartPos;
         int m_Length;
      };

      typedef SubTextR<const Text> ConstSubText;

      template <typename TextType>
      class SubTextRW : public SubTextR<TextType>
      {
      public:
         SubTextRW() : SubTextR<TextType>() {}
         SubTextRW(TextType& master, int startPos = 0,
                           int length = -1);
         void Replace(ConstSubText text);
      };

      typedef SubTextRW<Text> SubText;

      class Iterator
      {
      public:
         Iterator(const Text& text);
         Iterator(ConstSubText subText);
         ~Iterator() = default;
         Iterator(const Iterator& other) = default;
         Iterator& operator=(const Iterator& other) = default;
         void Next();
         unicode_t Get() const;
         int GetIndex() const;
         bool HasEnded() const;

         // Get how many bytes current codepoint needs to be encoded
         // returns 0 if end of text is reached or utf-8 text is invalid
         unsigned int GetNumBytes() const;

      private:

         // Do we have all necessary continuation bytes in the text?
         bool ValidateConBytes(unsigned int numBytes) const;

         // extracts and returns codepoint, doesn't do any checking
         unicode_t ExtractValue(unsigned int numBytes) const;

         // Extracts codepoint, does checks for size and validates utf-8
         void ExtractCodepoint(unsigned int numBytes);

         int GetMasterIndex() const; // This accounts for start offset
         bool IsEndReached() const;
      private:
         const Text* m_Text;
         int m_StartOffset, m_Length;
         int m_CurrIndex;
         unsigned int m_CurrByte;
         unicode_t m_CurrCodepoint;
         bool m_HasEnded;
      };

      template <typename SubTextType>
      class SubTextTypeVec
      {
      public:
         // preallocate some slots
         SubTextTypeVec(int availableSlots = 0);
         SubTextTypeVec(const SubTextTypeVec& other);
         ~SubTextTypeVec();

         SubTextTypeVec& operator=(const SubTextTypeVec& other);
         const SubTextType& operator[](int i) const;
         SubTextType& operator[](int i);

         void Add(SubTextType subText);
         int GetCount() const { return m_Count; }
      private:
         void EnsureSlotAvailable(int index);
      private:
         SubTextType* m_Vec;
         int m_AvailableSlots;
         int m_Count;
      };

      typedef SubTextTypeVec<ConstSubText> ConstSubTextVec;
      typedef SubTextTypeVec<SubText> SubTextVec;

      static bool IsBlank(unicode_t c);

      ConstSubTextVec GetWords() const;
      SubTextVec GetWords();

   private:
      // Get value of i-th byte in m_Data
      utf8_t operator[](unsigned int i) const;

   private:
      utf8_t* m_Data;
      unsigned int m_AvailableBytes;
      unsigned int m_OccupiedBytes;
   };



   // Template implementation SubTextReader
   
   template <typename TextType>
   Text::SubTextR<TextType>::
   SubTextR(TextType& master, int startPos, int length)
      : m_Text(&master)
   {
      int textLength = master.GetLength();
      if(startPos >= 0)
      {
         m_StartPos = startPos < textLength ? startPos : textLength - 1;
      }
      else
      {
         m_StartPos = textLength + startPos; // startPos here is negative
         if(m_StartPos < 0) m_StartPos = 0;
      }

      if(length < 0 || m_StartPos + length >= textLength)
      {
         m_Length = textLength - m_StartPos;
      }
      else
      {
         m_Length = length;
      }
   }
   
   template <typename TextType>
   Text::SubTextR<TextType>::
   SubTextR(Text::SubTextRW<Text> subText)
      : SubTextR(subText.GetMaster(), subText.GetStartPos(),
                  subText.GetLength())
   {}

   template <typename TextType>
   int Text::SubTextR<TextType>::
   GetByteLength() const
   {
      int res = 0;
      Iterator iter(*this);
      while(!iter.HasEnded())
      {
         res += iter.GetNumBytes();
         iter.Next();
      }
      return res;
   }

   template <typename TextType>
   bool Text::SubTextR<TextType>::
   operator==(const SubTextR& other) const
   {
      Iterator iterLHS(*this);
      Iterator iterRHS(other);
      while(!iterLHS.HasEnded() && !iterRHS.HasEnded())
      {
         if(iterLHS.Get() != iterRHS.Get())
            return false;
         iterLHS.Next();
         iterRHS.Next();
      }
      if(iterLHS.HasEnded() && iterRHS.HasEnded())
         return true;
      return false;
   }

   // Template implementation SubTextReaderWriter
   
   template <typename TextType>
   Text::SubTextRW<TextType>::
   SubTextRW(TextType& master, int startPos, int length)
      : SubTextR<TextType>(master, startPos, length)
   {}

   template <typename TextType>
   void Text::SubTextRW<TextType>::Replace(Text::ConstSubText text)
   {
      // TODO
   }

   // Template implementation SubTextTypeVec
  
   template <typename SubTextType>
   Text::SubTextTypeVec<SubTextType>::SubTextTypeVec(int availableSlots)
      : m_Vec(nullptr), m_Count(0)
   {
      const int minSlots = 4;
      m_AvailableSlots =
         availableSlots < minSlots ? minSlots : availableSlots;
      m_Vec = new SubTextType[m_AvailableSlots];
   }

   template <typename SubTextType>
   Text::SubTextTypeVec<SubTextType>::
   SubTextTypeVec(const SubTextTypeVec<SubTextType>& other)
   {
      m_AvailableSlots = other.m_AvailableSlots;
      m_Count = other.m_Count;
      if(m_AvailableSlots > 0)
      {
         m_Vec = new SubTextType[m_AvailableSlots];
      }
      for(int i = 0; i < other.m_Count; i++)
      {
         m_Vec[i] = other.m_Vec[i];
      }
   }

   template <typename SubTextType>
   Text::SubTextTypeVec<SubTextType>::~SubTextTypeVec()
   {
      if(m_Vec)
         delete [] m_Vec;
   }
 
   template <typename SubTextType>
   Text::SubTextTypeVec<SubTextType>& Text::SubTextTypeVec<SubTextType>::
   operator=(const SubTextTypeVec<SubTextType>& other)
   {
      if(m_AvailableSlots < other.m_Count)
      {
         if(m_Vec)
            delete [] m_Vec;
         m_Vec = new SubTextType[other.m_Count];
         m_AvailableSlots = other.m_Count;
      }
      for(int i = 0; i < other.m_Count; i++)
      {
         m_Vec[i] = other.m_Vec[i];
      }
      m_Count = other.m_Count;
   }

   template <typename SubTextType>
   const SubTextType& Text::SubTextTypeVec<SubTextType>::
   operator[](int i) const
   {
      return m_Vec[i];
   }

   template <typename SubTextType>
   SubTextType& Text::SubTextTypeVec<SubTextType>::
   operator[](int i)
   {
      EnsureSlotAvailable(i);
      return m_Vec[i];
   }

   template <typename SubTextType>
   void Text::SubTextTypeVec<SubTextType>::
   Add(SubTextType subText)
   {
      EnsureSlotAvailable(m_Count);
      m_Vec[m_Count] = subText;
      m_Count++;
   }

   template <typename SubTextType>
   void Text::SubTextTypeVec<SubTextType>::
   EnsureSlotAvailable(int index)
   {
      while(index >= m_AvailableSlots)
      {
         m_AvailableSlots *= 2;
      }
      SubTextType* newVec = new SubTextType[m_AvailableSlots];
      for(int i = 0; i < m_Count; i++)
      {
         newVec[i] = m_Vec[i];
      }
      if(m_Vec)
         delete [] m_Vec;
      m_Vec = newVec;
   }
}

#endif
