#ifndef _TOTEM_LOG_H_
#define _TOTEM_LOG_H_

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

enum class LogLevel { Info, Error, Fatal };

#ifdef TOTEM_DEBUG
   #ifdef PROJECT_NAME
      #define LOG_INFO(...) \
               printLog(LogLevel::Info, TOSTRING(PROJECT_NAME), __VA_ARGS__)
      #define LOG_ERROR(...) \
               printLog(LogLevel::Error, TOSTRING(PROJECT_NAME), __VA_ARGS__)
      #define LOG_FATAL(...) \
               printLog(LogLevel::Fatal, TOSTRING(PROJECT_NAME), __VA_ARGS__)
   #else
      #define LOG_INFO(...) \
               printLog(LogLevel::Info, "Client", __VA_ARGS__)
      #define LOG_ERROR(...) \
               printLog(LogLevel::Error, "Client", __VA_ARGS__)
      #define LOG_FATAL(...) \
               printLog(LogLevel::Fatal, "Client", __VA_ARGS__)
   #endif
#else
   #ifdef PROJECT_NAME
      #define LOG_INFO(...)
      #define LOG_ERROR(...) 
      #define LOG_FATAL(...) \
               printLog(LogLevel::Fatal, TOSTRING(PROJECT_NAME), __VA_ARGS__)
   #else
      #define LOG_INFO(...)
      #define LOG_ERROR(...)
      #define LOG_FATAL(...) \
               printLog(LogLevel::Fatal, "Client", __VA_ARGS__)
   #endif
#endif

void printLog(LogLevel level, const char *projectName, const char *fmt, ...);

#endif
