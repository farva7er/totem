#include "SimpleTextWriter.h"
#include "string.h"

namespace tgenerator
{
   SimpleTextWriter::SimpleTextWriter()
      : m_File(NULL), m_SrcFilePath(nullptr)
   {}

   SimpleTextWriter::~SimpleTextWriter()
   {
      if(m_File)
         fclose(m_File);
   }

   bool SimpleTextWriter::Open(const char* filePath)
   {
      if(m_File)
         fclose(m_File);
      
      char destFilePath[strlen(filePath) + 5];
      strcpy(destFilePath, filePath);
      strcat(destFilePath, ".ttr");
      m_File = fopen(destFilePath, "a");
      m_SrcFilePath = filePath;
      return m_File != NULL;
   }

   void SimpleTextWriter::Write(const TranslationUnit& unit)
   {
      const totem::utf8_t* str = unit.m_AdditionalInfo.GetRawData();
      int size = (int)unit.m_AdditionalInfo.GetRawSize();

      fprintf(m_File, "%s: ", m_SrcFilePath);
      fwrite(str, sizeof(totem::utf8_t), size, m_File);
      fputc('\n', m_File);

      str = unit.m_Text.GetRawData();
      size = (int)unit.m_Text.GetRawSize();

      fwrite(str, sizeof(totem::utf8_t), size, m_File);
      fputc('\n', m_File);
      fputc('\n', m_File);
   }   
}
