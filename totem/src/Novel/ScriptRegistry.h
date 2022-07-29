#ifndef _TOTEM_NOVEL_SCRIPT_REGISTRY_H_
#define _TOTEM_NOVEL_SCRIPT_REGISTRY_H_

#define SCRIPT_NO_CLIENT_USAGE
#include "Script.h"

namespace totem
{
   class Script;

   class ScriptRegistry
   {
      public:
         ScriptRegistry();
         ~ScriptRegistry();

         void AddScript(Script* script);

         Script& operator[](int i);
         const Script& operator[](int i) const;
      private:

         // Allocates twice more space
         void Reallocate();

      private:
         Script** m_Scripts;
         int m_Size;
         int m_Capacity;
   };
}

#endif
