#include "Tests.h"
#include "TextTests.h"

totem::App* totem::App::CreateApp()
{
   return new TestApp();
}

void TestApp::Run()
{
   TextTestsRun();
}
