#include <string.h>

#include "ResourceManager.h"
#include "Assert.h"
#include "Filesystem/Utils.h"

namespace totem
{
   Resource::Resource(const char* resPath)
   {
      m_Path = new char[sizeof(ResourceDir) + strlen(resPath)];
      strcpy(m_Path, ResourceDir);
      strcat(m_Path, resPath);
   }

   Resource::~Resource()
   {
      delete m_Path;
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
   {
      delete data; 
   }

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

   void ResourceManager::Insert(Resource* res)
   {
      if(m_ResourceCount >= GetSize()/2)
         ExtendTable();

      unsigned int hashIndex = HashString(res->GetPath());
      InsertNode(hashIndex, res);
      m_ResourceCount++;
   }

   void ResourceManager::Unload(const char* resPath)
   {
      unsigned int hashIndex = HashString(resPath);
      for(  ResourceNode** currNode = &m_Table[hashIndex];
            *currNode;
            currNode = &((*currNode)->next)
         )
      {
         if(0 == strcmp((*currNode)->data->GetPath(), resPath))
         {
            ResourceNode* savedNode = *currNode;
            *currNode = (*currNode)->next;
            delete savedNode;
            m_ResourceCount--;
            return;
         }
      }
   }

   Resource* ResourceManager::GetResourceInternal(const char* resPath) const
   {
      char* fullPath = new char[sizeof(ResourceDir) + strlen(resPath)];
      strcpy(fullPath, ResourceDir);
      strcat(fullPath, resPath);

      unsigned int hashIndex = HashString(fullPath);
      ResourceNode *currNode = m_Table[hashIndex];
      while(currNode)
      {
         if(0 == strcmp(currNode->data->GetPath(), fullPath))
         {
            LOG_INFO("Reusing resource: %s", resPath);
            delete fullPath;
            return currNode->data;
         }

         currNode = currNode->next;
      }
      delete fullPath;
      return nullptr;
   }

   void ResourceManager::LoadResourceInternal(Resource* res)
   {
      LOG_INFO("Loading resource: %s", res->GetPath());
      res->Load();
      Insert(res);
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
            Insert(curr->data);
            curr = curr->next;
         }
      }
      delete [] oldTable; 
   }
}
