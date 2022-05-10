#ifndef _TOTEM_TGENERATOR_H_
#define _TOTEM_TGENERATOR_H_

#include "Internationalization/Text.h"

namespace tgenerator
{
   struct TranslationUnit
   {
      totem::Text m_Text;
      totem::Text m_AdditionalInfo;
      TranslationUnit(const totem::Text& text,
                     const totem::Text& additionalInfo)
         : m_Text(text), m_AdditionalInfo(additionalInfo)
      {}
   };

   class Reader
   {
   public:
      virtual ~Reader() = default;
      virtual bool Open(const char* filePath) = 0;
      virtual bool HasEnded() const = 0;
      virtual TranslationUnit* GetTranslationUnit() = 0;
   };

   class Writer
   {
   public:
      virtual ~Writer() = default;
      virtual bool Open(const char* filePath) = 0;
      virtual void Write(const TranslationUnit& unit) = 0;
   };

   class Generator
   {
   public:
      Generator(Reader* r, Writer* w);
      ~Generator();
      Generator(const Generator& other) = delete;
      Generator& operator=(const Generator& other) = delete;

      // Generate a translation file for a given src file (script)
      void Generate(const char* srcFilePath);
   private:
      Reader* m_Reader;
      Writer* m_Writer;
   };
}

#endif

