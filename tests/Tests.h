#ifndef _TOTEM_TESTS_H_
#define _TOTEM_TESTS_H_

#include "App.h"
#include "Log.h"

#define TEST_ASSERT(x)\
{\
   if (!(x))\
   {\
      LOG_FATAL("Test Failed (%s) on line %d", __FUNCTION__, __LINE__);\
   }\
}

#define TEST_CASE(NAME)\
   struct TEST_STRUCT_##NAME\
   {\
      static void NAME()
       
#define TEST_END(NAME)\
   };\
   TEST_STRUCT_##NAME::NAME();

class TestApp : public totem::App
{
public:
   TestApp(int argc, char** argv) : totem::App(argc, argv) {}
   virtual void Run() override;
};

#endif
