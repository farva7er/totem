#include "Tests.h"
#include "Log.h"

totem::App* totem::App::CreateApp()
{
   return new TestApp();
}

void TestApp::Run()
{
   LOG_INFO("Hello, tests passed!");
}
