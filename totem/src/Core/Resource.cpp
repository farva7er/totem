#include "Resource.h"
#include "ResourceManager.h"

namespace totem
{
   Resource::Resource(const char* name, ResourceManager* manager)
      : m_Manager(manager), m_RefCount(0), m_IsLoaded(false)
   {
      if(name)
      {
         m_Name = new char[strlen(name) + 1];
         strcpy(m_Name, name);
      }
      else
      {
         m_Name = nullptr;
      }
   }

   void Resource::Release()
   {
      m_Manager->Release(this);
   }

   Resource::~Resource()
   {
      if(m_Name)
         delete [] m_Name;
   }
}
