#include "App.h"
#include "Log.h"
#include "Assert.h"

int main()
{
   totem::App* app = totem::App::CreateApp();
   TOTEM_ASSERT(true, "TEST assertion");
   app->Run();
   delete app;
   LOG_INFO("Good bye!");
   return 0;
}
