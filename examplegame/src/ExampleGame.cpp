#include "Novel/NovelApp.h"
#include "Novel/Script.h"

class FirstLevel : public totem::Script
{
public:
   void Play()
   {
      Bg("resources/bg.jpg");
      Say(  "You're welcome to totem! It's a visual novel engine "
            "written in C++. Or at least it will be in future:) "
            "It's in development and supports little functionality, "
            "but it will support much more in near future. "
            "Click left mouse button to go to next message."
         );
      Say(  "It's an example game which demonstrates basic "
            "functionality the engine supports.");
      Say(  "The engine supports different languages. "
            "Internally it uses UTF-8.");

      // Shouldn't really use non-ASCII here,
      // but for now it'll work
      Say(  "Добро пожаловать в totem!");
      Say(  "After this message the program should close. Have a nice day!");
   }
};

class ExampleGame : public totem::NovelApp
{
public:
   void Run()
   {
      FirstLevel level1;
      level1.Play();
   }
};

totem::App* totem::App::CreateApp(int argc, char** argv)
{
   return new ExampleGame();
}
