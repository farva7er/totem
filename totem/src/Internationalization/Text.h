#ifndef _INTERNATIONALIZATION_TEXT_H_
#define _INTERNATIONALIZATION_TEXT_H_

namespace totem
{

   typedef unsigned char utf8_t;
   typedef unsigned int unicode_t;

   /* Text class represents a UTF-8 encoded string. 
    *
    * The Text class has 3 nested classes: Iterator, SubText, SubTextVec.
    * SubText also has a const form - ConstSubText, which can be
    * stored in ConstSubTextVec.
    *
    * SubText represents a lightweight object which references a Text
    * object and stores a starting position and length of a SubText.
    * SubText can be implicitly constructed from a Text - it's just
    * a subText with starting position 0 and length equal to length of
    * the Text
    *
    * Iterator is used for iterarting over codepoints in SubText sequentially.
    * Text does not support random access since UTF-8 is a varible-width
    * encoding.
    *
    * SubTextVec is a vector that stores SubText's. It's useful as a
    * return value for operations like word splitting of a Text.
    *
    */

   class Text
   {
      public:

         /* To avoid code duplication in having const and non-const
          * version of SubText it is separated in two
          * classes, one of which inherits from the other.
          * SubTextR (Read Only) - provides only const methods
          * which cannot modify referenced Text object.
          * SubTextRW (Read And Write) inherits from SubTextR and
          * additionally provides methods that modify the Text.
          *
          * If we need to make a substring of a const Text, then
          * we should use SubTextR with template type const Text.
          * If we need to make a substring of a Text, then
          * we should use SubTextRW with template type Text.
          * Two typedef's are provided for convenience and to avoid
          * misusing these templates with wrong types.
          *
          * DO NOT USE THESE TEMPLATES DIRECTLY, USE
          * ConstSubText AND SubText TYPES ONLY!
          */

         template <typename TextType>
         class SubTextR;
         typedef SubTextR<const Text> ConstSubText;

         template <typename TextType>
         class SubTextRW;
         typedef SubTextRW<Text> SubText;

         template <typename TextType>
         class TextTypeVec;

         typedef TextTypeVec<ConstSubText> ConstSubTextVec;
         typedef TextTypeVec<SubText> SubTextVec;
         typedef TextTypeVec<Text> TextVec;

         class Iterator;


         // Check if c is SP, HTAB, CR or LF.
         static bool IsBlank(unicode_t c);

         // Default constuctor creates empty Text.
         Text();

         // Construct Text from a null-terminated ASCII string.
         Text(const char* c_str);

         // Construct Text from a given utf-8 buffer.
         Text(const utf8_t* str, int sizeInBytes);

         // Constructor with an optimisation hint to reserve some bytes.
         Text(int reserveBytes);

         Text(const Text& other);

         ~Text();

         Text& operator=(const Text& other);

         Text& operator+=(char c);
         Text& operator+=(ConstSubText other);

         utf8_t* GetRawData();
         const utf8_t* GetRawData() const;
         int GetByteCount() const;
         int GetLength() const;
         bool IsEmpty() const;
    
         ConstSubTextVec GetWords() const;
         SubTextVec GetWords();


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

            int GetStartPos() const { return m_StartPos; }
            int GetLength() const { return m_Length; }
            int GetByteCount() const;

            TextType& GetMaster() const { return *m_Text; }
            Text GetText() const;
    
         private:
            TextType* m_Text;
            int m_StartPos;
            int m_Length;
         };


         template <typename TextType>
         class SubTextRW : public SubTextR<TextType>
         {
         public:
            SubTextRW() : SubTextR<TextType>() {}
            SubTextRW(TextType& master, int startPos = 0,
                              int length = -1);

            // TODO: implement
            // Replace this SubText with a specified text
            void Replace(ConstSubText text);
         };


         class Iterator
         {
         public:
            // This constructor is needed to avoid dependency cycle:
            // to construct a SubText it is necessary to calculate
            // length of the corresponding text using an iterator.
            Iterator(const Text& text);
            Iterator(ConstSubText subText);
            Iterator(const Iterator& other) = default;

            ~Iterator() = default;

            Iterator& operator=(const Iterator& other) = default;

            void Next();

            // Returns 4-byte value - codepoint
            unicode_t Get() const;
            // Returns current index
            int GetIndex() const;
            // Returns index of a first byte which encodes current codepoint
            int GetCurrByteIndex() const;

            bool HasEnded() const;

            // Get how many bytes current codepoint needs to be encoded.
            // Returns 0 if end of text is reached or utf-8 text is invalid
            int GetNumBytes() const;

         private:
            // Do we have all necessary continuation bytes in the text?
            bool ValidateConBytes(int numBytes) const;
            // extracts and returns codepoint, doesn't do any checking
            unicode_t ExtractValue(int numBytes) const;
            // Extracts codepoint, does checks for size and validates utf-8
            void ExtractCodepoint(int numBytes);
            // Get index relative to beginning of the text
            int GetMasterIndex() const; 
            bool IsEndReached() const;
            void SetEnded();

         private:
            const Text* m_Text;
            int m_StartOffset, m_Length;
            int m_LastExtractedIndex;
            int m_CurrByte;
            unicode_t m_CurrCodepoint;
            bool m_HasEnded;
         };

         template <typename TextType>
         class TextTypeVec
         {
         public:
            // preallocate some slots
            TextTypeVec(int availableSlots = 0);
            TextTypeVec(const TextTypeVec& other);
            ~TextTypeVec();

            TextTypeVec& operator=(const TextTypeVec& other);
            const TextType& operator[](int i) const;
            TextType& operator[](int i);

            void Add(TextType subText);
            int GetCount() const { return m_Count; }
            void Clear();
         private:
            void EnsureSlotAvailable(int index);
         private:
            TextType* m_Vec;
            int m_AvailableSlots;
            int m_Count;
         };
    
      private:
         // Get value of i-th byte in m_Data
         utf8_t operator[](int i) const;

         // Calculates and sets m_Length
         void CalculateLength();

      private:
         utf8_t* m_Data;
         int m_AvailableBytes;
         int m_OccupiedBytes;
         int m_Length;
   };

   Text operator+(Text::ConstSubText a, Text::ConstSubText b);
   bool operator==(Text::ConstSubText a, Text::ConstSubText b);




///////////////////////////////////////////////////////////////////
///////// TEMPLATE MEMBER FUNCTIONS IMPLEMENTATIONS ///////////////
///////////////////////////////////////////////////////////////////

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
   GetByteCount() const
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
   Text Text::SubTextR<TextType>::
   GetText() const
   {
      int startByteIndex;
      Iterator iter(*this);  
      startByteIndex = iter.GetCurrByteIndex();
      return Text(m_Text->m_Data + startByteIndex, GetByteCount());
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

   // Template implementation TextTypeVec
  
   template <typename TextType>
   Text::TextTypeVec<TextType>::TextTypeVec(int availableSlots)
      : m_Vec(nullptr), m_Count(0)
   {
      const int minSlots = 4;
      m_AvailableSlots =
         availableSlots < minSlots ? minSlots : availableSlots;
      m_Vec = new TextType[m_AvailableSlots];
   }

   template <typename TextType>
   Text::TextTypeVec<TextType>::
   TextTypeVec(const TextTypeVec<TextType>& other)
   {
      m_AvailableSlots = other.m_AvailableSlots;
      m_Count = other.m_Count;
      if(m_AvailableSlots > 0)
      {
         m_Vec = new TextType[m_AvailableSlots];
      }
      for(int i = 0; i < other.m_Count; i++)
      {
         m_Vec[i] = other.m_Vec[i];
      }
   }

   template <typename TextType>
   Text::TextTypeVec<TextType>::~TextTypeVec()
   {
      if(m_Vec)
         delete [] m_Vec;
   }
 
   template <typename TextType>
   Text::TextTypeVec<TextType>& Text::TextTypeVec<TextType>::
   operator=(const TextTypeVec<TextType>& other)
   {
      if(m_AvailableSlots < other.m_Count)
      {
         if(m_Vec)
            delete [] m_Vec;
         m_Vec = new TextType[other.m_Count];
         m_AvailableSlots = other.m_Count;
      }
      for(int i = 0; i < other.m_Count; i++)
      {
         m_Vec[i] = other.m_Vec[i];
      }
      m_Count = other.m_Count;
   }

   template <typename TextType>
   const TextType& Text::TextTypeVec<TextType>::
   operator[](int i) const
   {
      return m_Vec[i];
   }

   template <typename TextType>
   TextType& Text::TextTypeVec<TextType>::
   operator[](int i)
   {
      EnsureSlotAvailable(i);
      return m_Vec[i];
   }

   template <typename TextType>
   void Text::TextTypeVec<TextType>::
   Add(TextType subText)
   {
      EnsureSlotAvailable(m_Count);
      m_Vec[m_Count] = subText;
      m_Count++;
   }

   template <typename TextType>
   void Text::TextTypeVec<TextType>::
   Clear()
   {
      m_Count = 0;
   }

   template <typename TextType>
   void Text::TextTypeVec<TextType>::
   EnsureSlotAvailable(int index)
   {
      while(index >= m_AvailableSlots)
      {
         m_AvailableSlots *= 2;
      }
      TextType* newVec = new TextType[m_AvailableSlots];
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
