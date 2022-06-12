#include "App.h"
#include "Log.h"
#include "Assert.h"

int main(int argc, char** argv)
{
   totem::App* app = totem::App::CreateApp(argc, argv);
   app->Run();
   delete app;
   LOG_INFO("Main Exit");
   return 0;
}
