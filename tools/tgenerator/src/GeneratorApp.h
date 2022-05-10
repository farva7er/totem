#ifndef _TOTEM_GENERATOR_APP_H_
#define _TOTEM_GENERATOR_APP_H_

#include "App.h"

namespace tgenerator
{
   class GeneratorApp : public totem::App
   {
   public:
      GeneratorApp(int argc, char** argv) : totem::App(argc, argv) {}
      virtual void Run() override;
   };
}

totem::App* totem::App::CreateApp(int argc, char** argv)
{
   return new tgenerator::GeneratorApp(argc, argv);
}

#endif
