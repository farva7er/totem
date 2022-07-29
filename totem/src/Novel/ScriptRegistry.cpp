#include "ScriptRegistry.h"

namespace totem
{
   ScriptRegistry::ScriptRegistry()
   {
      m_Capacity = 2;
      m_Size = 0;
      m_Scripts = new Script*[m_Capacity];
   }

   ScriptRegistry::~ScriptRegistry()
   {
      delete [] m_Scripts;
   }
 
   void ScriptRegistry::AddScript(Script* script)
   {
      if(m_Size >= m_Capacity)
         Reallocate();

      m_Scripts[m_Size] = script;
   }

   Script& ScriptRegistry::operator[](int i)
   {
      return *(m_Scripts[i]);
   }

   const Script& ScriptRegistry::operator[](int i) const
   {
      return *(m_Scripts[i]);
   }

   void ScriptRegistry::Reallocate()
   {
      m_Capacity *= 2;
      Script** newArr = new Script*[m_Capacity];
      for(int i = 0; i < m_Size; i++)
         newArr[i] = m_Scripts[i];
      delete [] m_Scripts;
      m_Scripts = newArr;
   }
}
