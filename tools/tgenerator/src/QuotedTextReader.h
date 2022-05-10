#ifndef _TOTEM_QUOTED_TEXT_READER_H_
#define _TOTEM_QUITED_TEXT_READER_H_

#include "Generator.h"
#include <stdio.h>

namespace tgenerator
{
   class QuotedTextReader : public Reader
   {
   public:
      QuotedTextReader();
      virtual ~QuotedTextReader();
      QuotedTextReader(const QuotedTextReader& other) = delete;
      QuotedTextReader& operator=(const QuotedTextReader& other) = delete;

      virtual bool Open(const char* filePath) override;
      virtual bool HasEnded() const override;
      virtual TranslationUnit* GetTranslationUnit() override;

   private:
      void ExtractTranslationUnits();
      void ClearTranslationUnits();
      void AddTranslationUnit(int lineNumber);
      void Next();

   private:
      const char* m_FilePath;
      FILE* m_File;

      struct TranslationUnitNode
      {
         TranslationUnit* m_Unit;
         TranslationUnitNode* m_Next;
         TranslationUnitNode(TranslationUnit* unit,
                              TranslationUnitNode* next = nullptr)
            : m_Unit(unit), m_Next(next)
         {}

         ~TranslationUnitNode() { delete m_Unit; if(m_Next) delete m_Next; }
      };

      TranslationUnitNode* m_Units;
   };
}

#endif
