#ifndef _TOTEM_TIMER_H_
#define _TOTEM_TIMER_H_
namespace totem
{
   class Timer
   {
      public:
         static double GetTimeSec();

         static unsigned int SecToMilli(double secs)
         {  return static_cast<unsigned int>(secs * 1000); }
   };
}
#endif
