#ifndef _TOTEM_SIMPLE_TEXT_WRITER_H_
#define _TOTEM_SIMPLE_TEXT_WRITER_H_

#include "Generator.h"
#include <stdio.h>

namespace tgenerator
{
   class SimpleTextWriter : public Writer
   {
   public:
      SimpleTextWriter();
      virtual ~SimpleTextWriter();
      virtual bool Open(const char* filePath) override;
      virtual void Write(const TranslationUnit& unit) override;
   
   private:
      FILE* m_File;
      const char* m_SrcFilePath;
   };
}

#endif
