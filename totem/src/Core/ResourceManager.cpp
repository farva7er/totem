#include <string.h>

#include "ResourceManager.h"
#include "Assert.h"

namespace totem
{
   // Prime numbers close to powers of 2.
   const static unsigned int TableSizes[] = 
   {
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
      m_HashTable = new ResourceNode*[TableSizes[m_SizeIndex]];
      for(unsigned int i = 0; i < TableSizes[m_SizeIndex]; i++)
      {
         m_HashTable[i] = nullptr;
      }
   }

   ResourceManager::~ResourceManager()
   {
      ResourceNode* curr;
      for(unsigned int i = 0; i < GetSize(); i++)
      {
         curr = m_HashTable[i];
         while(curr)
         {
            ResourceNode* savedNode = curr;
            curr = curr->next;
            delete savedNode;
         }
      }
      delete [] m_HashTable;
   }

   ResourceManager::ResourceNode::~ResourceNode()
   {
      LOG_INFO("Freeing Resource: %s", data->GetName());
      delete data;
   }
 
   unsigned int ResourceManager::GetSize() const
   {
      return TableSizes[m_SizeIndex];
   }

   void ResourceManager::InsertNode(unsigned int index, Resource* res)
   {
      ResourceNode* resNode = new ResourceNode(res, m_HashTable[index]);
      m_HashTable[index] = resNode;
   }

   void ResourceManager::AddResource(Resource* res)
   {
      if(m_ResourceCount >= GetSize()/2)
         ExtendTable();

      unsigned int hashIndex = HashResName(res->GetName());
      InsertNode(hashIndex, res);
      m_ResourceCount++;
   }
 
   void ResourceManager::Release(Resource* res)
   {
      res->DecRef();
      if(res->GetRef() <= 0)
      {
         FreeResource(res->GetName());
      }
   }

   void ResourceManager::FreeResource(const char* resName)
   {
      unsigned int hashIndex = HashResName(resName);
      for(  ResourceNode** currNode = &m_HashTable[hashIndex];
            *currNode;
            currNode = &((*currNode)->next)
         )
      {
         if(0 == strcmp((*currNode)->data->GetName(), resName))
         {
            ResourceNode* savedNode = *currNode;
            *currNode = (*currNode)->next;
            delete savedNode;
            m_ResourceCount--;
            return;
         }
      }
   }

   unsigned int ResourceManager::HashResName(const char* resName)
   {
      unsigned int result = 0;
      while(*resName)
      {
         // 31 - is a prime close to alphabet size
         result = 31 * result + *resName;
         resName++;
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
      ResourceNode** oldTable = m_HashTable;
      m_HashTable = extTable;

      for(unsigned int i = 0; i < TableSizes[m_SizeIndex - 1]; i++)
      {
         ResourceNode *curr = oldTable[i];
         while(curr)
         {
            AddResource(curr->data);
            curr = curr->next;
         }
      }
      delete [] oldTable; 
   }
}
