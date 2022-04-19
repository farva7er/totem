#ifndef _TOTEM_APP_H_
#define _TOTEM_APP_H_

namespace totem
{
   class App
   {
   public:
      App() = default;
      virtual ~App() {};
      App(const App& other) = delete;
      App& operator=(const App& other) = delete;

      // To Be defined by client project
      static App* CreateApp();

      virtual void Run() = 0;
   };
}

#endif
