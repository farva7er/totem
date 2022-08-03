#ifndef _TOTEM_NOVEL_CLIENT_API_H_
#define _TOTEM_NOVEL_CLIENT_API_H_

#define TryYield() { if(ScriptShouldYield()) return; }

// Script API

#define Wait() Wait_L();TryYield()
#define GetOption(options) GetOption_L(options);TryYield()

// Character API

#define Say(text) Say_L(text);TryYield()
#define Show(slot) Show_L(slot);TryYield()
#define Hide() Hide_L();TryYield()

//

#endif
