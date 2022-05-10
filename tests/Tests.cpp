#include "Tests.h"
#include "TextTests.h"

totem::App* totem::App::CreateApp(int argc, char** argv)
{
   return new TestApp(argc, argv);
}

void TestApp::Run()
{
   TextTestsRun();
}
