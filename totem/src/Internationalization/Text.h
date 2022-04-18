#ifndef _INTERNATIONALIZATION_TEXT_H_
#define _INTERNATIONALIZATION_TEXT_H_

namespace totem
{

   typedef char utf8_t;
   typedef unsigned int unicode_t;

   class Text
   {
   public:
      //Construct Text from a given utf8 buffer
      Text(const utf8_t* str, unsigned int sizeInBytes);

      // An optimisation hint, reserve some bytes upfront
      Text(unsigned int reserveBytes);
      ~Text();

      Text(const Text& other);
      Text& operator=(const Text& other);
 
      class Iterator
      {
      public: 
         Iterator(const Text* master);
         ~Iterator() = default;
         Iterator(const Iterator& other) = default;
         Iterator& operator=(const Iterator& other) = default;
         void Next();
         unicode_t Get() const; 
         bool HasEnded() const;
      private:
         // Get how many bytes current codepoint needs to be encoded
         // returns 0 if end of text is reached or utf-8 text is invalid
         unsigned int GetNumBytes() const;

         // Do we have all necessary continuation bytes in the text?
         bool ValidateConBytes(unsigned int numBytes) const;

         // extracts and returns codepoint, doesn't do any checking
         unicode_t ExtractValue(unsigned int numBytes) const;

         // Extracts codepoint, does checks for size and validates utf-8
         void ExtractCodepoint(unsigned int numBytes);
      private:
         const Text* m_Master;
         unsigned int m_CurrByte;
         unicode_t m_CurrCodepoint;
         bool m_HasEnded;
      };

      Iterator GetIterator() const;

   private:
      // Get value of i-th byte in m_Data
      utf8_t operator[](unsigned int i) const;

   private:
      utf8_t* m_Data;
      unsigned int m_AvailableBytes;
      unsigned int m_OccupiedBytes;

   };
}

#endif

