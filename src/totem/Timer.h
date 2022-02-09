#ifndef _TOTEM_TIMER_H_
#define _TOTEM_TIMER_H_

#include <GLFW/glfw3.h>

class Timer
{

public:
   static double GetTimeSec()
   { return glfwGetTime(); }

   static unsigned int SecToMilli(double secs)
   {  return static_cast<unsigned int>(secs * 1000); }
};

#endif
