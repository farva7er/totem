#include <string.h>

#include "ResourceManager.h"
#include "Assert.h"
#include "Filesystem/Utils.h"

namespace totem
{
   Resource::Resource(const char* resId)
      : m_IsLoaded(false)
   {
      m_ResourceId = new char[strlen(resId) + 1];
      strcpy(m_ResourceId, resId);
   }

   Resource::~Resource()
   {
      delete m_ResourceId;
   }

   ResourceType Resource::GetType() const
   { return ResourceType::Other;   }

   /////////////////////////////////////////////////////
   //////////Resource Manager///////////////////////////
   /////////////////////////////////////////////////////

   ResourceManager* ResourceManager::s_Instance = nullptr;

   const static unsigned int TableSizes[] = {
      53,
      97,
      193,
      389,
      769,
      1543,
      3079,
      6151,
      12289,
      24593,
      49157,
      98317,
      196613
   };

   ResourceManager::ResourceManager()
      : m_SizeIndex(0), m_ResourceCount(0)
   {
      m_Table = new ResourceNode*[TableSizes[m_SizeIndex]];
      for(unsigned int i = 0; i < TableSizes[m_SizeIndex]; i++)
      {
         m_Table[i] = nullptr;
      }
      FileSystemHelper::Get().SetWorkingDirToExePath();
   }

   ResourceManager::~ResourceManager()
   {
      ResourceNode* curr;
      for(unsigned int i = 0; i < GetSize(); i++)
      {
         curr = m_Table[i];
         while(curr)
         {
            ResourceNode* savedNode = curr;
            curr = curr->next;
            delete savedNode;
         }
      }      
   }

   ResourceManager::ResourceNode::~ResourceNode()
   { delete data; }

   ResourceManager& ResourceManager::GetInstance()
   {
      if(!s_Instance)
         s_Instance = new ResourceManager();
      return *s_Instance;
   }

   void ResourceManager::DeleteInstance()
   {
      delete s_Instance;
      s_Instance = nullptr;
   }

   unsigned int ResourceManager::GetSize() const
   {
      return TableSizes[m_SizeIndex];
   }

   void ResourceManager::InsertNode(unsigned int index, Resource* res)
   {
      ResourceNode* resNode = new ResourceNode(res, m_Table[index]);
      m_Table[index] = resNode;
   }

   void ResourceManager::AddResourceInternal(Resource* res)
   {
      if(m_ResourceCount >= GetSize()/2)
         ExtendTable();

      unsigned int hashIndex = HashString(res->GetId());
      InsertNode(hashIndex, res);
      m_ResourceCount++;
   }

   void ResourceManager::Unload(const char* resId)
   {
      unsigned int hashIndex = HashString(resId);
      for(  ResourceNode** currNode = &m_Table[hashIndex];
            *currNode;
            currNode = &((*currNode)->next)
         )
      {
         if(0 == strcmp((*currNode)->data->GetId(), resId))
         {
            ResourceNode* savedNode = *currNode;
            *currNode = (*currNode)->next;
            delete savedNode;
            m_ResourceCount--;
            return;
         }
      }
   }

   Resource* ResourceManager::GetResourceInternal(const char* resId) const
   {
      unsigned int hashIndex = HashString(resId);
      ResourceNode *currNode = m_Table[hashIndex];
      while(currNode)
      {
         if(0 == strcmp(currNode->data->GetId(), resId))
         {
            //LOG_INFO("Reusing resource: %s", resId);
            Resource* res = currNode->data;
            if(!res->IsLoaded())
            {
               res->Load();
               LOG_INFO("Loading resource: %s", res->GetId());
            }
            return currNode->data;
         }
         currNode = currNode->next;
      }
      return nullptr;
   }

   void ResourceManager::LoadResourceInternal(Resource* res)
   {
      LOG_INFO("Loading resource: %s", res->GetId());
      res->Load();
      AddResourceInternal(res);
   }

   unsigned int ResourceManager::HashString(const char* str) const
   {
      unsigned int result = 0;
      while(*str)
      {
         result = 31 * result + *str; // 31 - is a prime close to alphabet size
         str++;
      }
      return result % TableSizes[m_SizeIndex]; 
   }

   void ResourceManager::ExtendTable()
   {
      TOTEM_ASSERT(( unsigned int)m_SizeIndex + 1 < 
            sizeof(TableSizes)/sizeof(unsigned int), 
            "Too many resources!");

      m_SizeIndex++;
      ResourceNode** extTable = new ResourceNode*[TableSizes[m_SizeIndex]];
      ResourceNode** oldTable = m_Table;
      m_Table = extTable;

      for(unsigned int i = 0; i < TableSizes[m_SizeIndex - 1]; i++)
      {
         ResourceNode *curr = oldTable[i];
         while(curr)
         {
            AddResourceInternal(curr->data);
            curr = curr->next;
         }
      }
      delete [] oldTable; 
   }
}
