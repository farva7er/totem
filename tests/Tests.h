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


class TestApp : public totem::App
{
public:
   virtual void Run() override;
};

#endif
