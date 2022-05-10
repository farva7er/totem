#include "GeneratorApp.h"
#include "Generator.h"
#include "QuotedTextReader.h"
#include "SimpleTextWriter.h"

namespace tgenerator
{
   void GeneratorApp::Run()
   {
      if(GetArgc() < 2)
      {
         fprintf(stderr, "Please specify src filepath\n");
         return;
      }

      Generator generator(new QuotedTextReader(), new SimpleTextWriter());
      generator.Generate(GetArgv()[1]);
   }
}
