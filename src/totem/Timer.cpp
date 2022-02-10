#include "Timer.h"

#include <GLFW/glfw3.h>

namespace totem
{
   double Timer::GetTimeSec() { return glfwGetTime(); }
}
