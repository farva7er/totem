#include "QuotedTextReader.h"
#include <string.h>
#include "Internationalization/Text.h"

namespace tgenerator
{
   QuotedTextReader::QuotedTextReader()
      : m_FilePath(nullptr), m_File(NULL), m_Units(nullptr)
   {}

   QuotedTextReader::~QuotedTextReader()
   {
      if(m_File)
         fclose(m_File);
      ClearTranslationUnits();
   }

   bool QuotedTextReader::Open(const char* filePath)
   {
      if(m_File)
      {
         fclose(m_File);
      }

      ClearTranslationUnits();
      m_FilePath = filePath;

      m_File = fopen(filePath, "r");
      if(m_File)
      {
         ExtractTranslationUnits();
      }

      return m_File != NULL;
   }

   bool QuotedTextReader::HasEnded() const
   {
      return m_Units == nullptr;
   }

   void QuotedTextReader::Next()
   {
      if(m_Units)
         m_Units = m_Units->m_Next;
   }

   TranslationUnit* QuotedTextReader::GetTranslationUnit()
   {
      TranslationUnit* unit = m_Units->m_Unit;
      Next();
      return unit;
   }

   void QuotedTextReader::ExtractTranslationUnits()
   {
      enum { buffSize = 1024 };
      char buff[buffSize];

      bool isQuoteOpen = false;
      int currLineNumber = 1;

      while(fgets(buff, buffSize, m_File))
      {
         for(int i = 0; i < buffSize && buff[i]; i++)
         {
            if(buff[i] == '\n')
               currLineNumber++;

            if(buff[i] == '\"')
            {
               isQuoteOpen = !isQuoteOpen;
               if(isQuoteOpen)
               {
                  AddTranslationUnit(currLineNumber);
               }
               continue;
            }

            if(isQuoteOpen)
            {
               m_Units->m_Unit->m_Text += buff[i];
            }
         }
      }
   }

   void QuotedTextReader::AddTranslationUnit(int lineNumber)
   {
      char str[10];
      snprintf(str, 10, "%d", lineNumber);
                           // Text does not need to store the null byte
      m_Units = new TranslationUnitNode(
                     new TranslationUnit(totem::Text(),
                           totem::Text((totem::utf8_t*)str, strlen(str))),
                     m_Units);
   }

   void QuotedTextReader::ClearTranslationUnits()
   {
      while(m_Units)
      {
         TranslationUnitNode* savedNode = m_Units;
         m_Units = m_Units->m_Next;
         delete savedNode;
      }
   }
}
