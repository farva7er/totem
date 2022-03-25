#ifndef _TOTEM_LOG_H_
#define _TOTEM_LOG_H_

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#include <string.h>
#define __FILENAME__ \
   (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)


enum class LogLevel { Info, Error, Fatal };

#ifdef TOTEM_DEBUG
   #ifdef PROJECT_NAME
      #define LOG_INFO(...) \
               printLog(LogLevel::Info, __FILENAME__, __LINE__, \
                        TOSTRING(PROJECT_NAME), __VA_ARGS__)
      #define LOG_ERROR(...) \
               printLog(LogLevel::Error, __FILENAME__, __LINE__, \
                        TOSTRING(PROJECT_NAME), __VA_ARGS__)
      #define LOG_FATAL(...) \
               printLog(LogLevel::Fatal, __FILENAME__, __LINE__, \
                        TOSTRING(PROJECT_NAME), __VA_ARGS__)
   #else
      #define LOG_INFO(...) \
               printLog(LogLevel::Info, __FILENAME__, __LINE__, \
                     "Client", __VA_ARGS__)
      #define LOG_ERROR(...) \
               printLog(LogLevel::Error, __FILENAME__, __LINE__, \
                     "Client", __VA_ARGS__)
      #define LOG_FATAL(...) \
               printLog(LogLevel::Fatal, __FILENAME__, __LINE__, \
                     "Client", __VA_ARGS__)
   #endif
#else
   #ifdef PROJECT_NAME
      #define LOG_INFO(...)
      #define LOG_ERROR(...) 
      #define LOG_FATAL(...) \
               printLog(LogLevel::Fatal, __FILENAME__, __LINE__, \
                        TOSTRING(PROJECT_NAME), __VA_ARGS__)
   #else
      #define LOG_INFO(...)
      #define LOG_ERROR(...)
      #define LOG_FATAL(...) \
               printLog(LogLevel::Fatal, __FILENAME__, __LINE__, \
                     "Client", __VA_ARGS__)

   #endif
#endif

void printLog(LogLevel level, const char* filename, int line,
               const char *projectName, const char *fmt, ...);

#endif
