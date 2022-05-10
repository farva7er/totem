#include "Generator.h"
#include "stdio.h"

namespace tgenerator
{
   Generator::Generator(Reader* r, Writer* w)
      : m_Reader(r), m_Writer(w)
   {}

   Generator::~Generator()
   {
      delete m_Reader;
      delete m_Writer;
   }

   void Generator::Generate(const char* srcFilePath)
   {
      m_Reader->Open(srcFilePath);
      m_Writer->Open(srcFilePath);
      while(!m_Reader->HasEnded())
      {
         TranslationUnit* unit = m_Reader->GetTranslationUnit();
         m_Writer->Write(*unit);
      }
   }
}
