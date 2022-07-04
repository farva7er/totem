#ifndef _TOTEM_APP_H_
#define _TOTEM_APP_H_

namespace totem
{
   class App
   {
      public:
         App(int argc, char** argv) : m_Argc(argc), m_Argv(argv) {}
         virtual ~App() {};
         App(const App& other) = delete;
         App& operator=(const App& other) = delete;

         int GetArgc() const { return m_Argc; }
         char** GetArgv() const { return m_Argv; }

         // To Be defined by client project
         static App* CreateApp(int argc, char** argv);

         virtual void Run() = 0;
      private:
         int m_Argc;
         char** m_Argv;
   };
}

#endif
