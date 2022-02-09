#include <stdio.h>
#include <stdarg.h>
#include <ctime>
#include "Log.h"
#include <iostream>

void printLog(LogLevel level, const char *projectName, const char *fmt, ...)
{
   va_list vl;
   std::tm curr_time;
   time_t time_since_epoch = time(nullptr);
   curr_time = *(localtime(&time_since_epoch));
   printf("%02d:%02d ", curr_time.tm_hour, curr_time.tm_min);

   switch(level) {
      case LogLevel::Info: printf("[Info] "); break;
      case LogLevel::Error: printf("[Error] "); break;
      case LogLevel::Fatal: printf("[FATAL] "); break;
   }
   printf("%s: ", projectName);
   va_start(vl, fmt);
   vprintf(fmt, vl);
   va_end(vl);
   printf("\n");
}


