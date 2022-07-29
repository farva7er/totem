#include "Novel/NovelApp.h"
#include "Novel/Script.h"

class FirstLevel : public totem::Script
{
public:
   void Play()
   {
      totem::Character myChar("totemia", "totemia");
      myChar.SetNameColor({0.2f, 0.8f, 0.3f, 1.0f});
      Bg("resources/bg.jpg");
      myChar.Say(  "You're welcome to totem! It's a visual novel engine "
            "written in C++. Or at least it will be in future:) "
            "It's in development and supports little functionality, "
            "but it will support much more in near future. "
            "Click left mouse button to go to next message."
         );
      myChar.Show(1);
      myChar.Say(  "It's an example game which demonstrates basic "
            "functionality the engine supports.");
      myChar.Say(  "The engine supports different languages. "
            "Internally it uses UTF-8.");
      myChar.SetMood("Laugh");

      // Shouldn't really use non-ASCII here,
      // but for now it'll work
      myChar.Say(  "Добро пожаловать в totem!");
      myChar.Hide();
      myChar.Say(  "After this message the program should"
                     " close. Have a nice day!");
   }
};

class ExampleGame : public totem::NovelApp
{
public:
   void Run()
   {
      FirstLevel level1;
      AddScript(level1);
      
      StartMainMenu();
   }
};

totem::App* totem::App::CreateApp(int argc, char** argv)
{
   return new ExampleGame();
}
