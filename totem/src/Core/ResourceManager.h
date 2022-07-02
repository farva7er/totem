#ifndef _TOTEM_RESOURCE_MANAGER_H_
#define _TOTEM_RESOURCE_MANAGER_H_

#include "Resource.h"
#include "Log.h"

#include <string.h>

namespace totem
{   
   // ResourceManager stores resources.
   // It is implemented as a basic hash table with chaining.
   // You can create as many ResourceManager objects as you
   // want (it is not a singleton).
   class ResourceManager
   {
      public:
         ResourceManager();
         ~ResourceManager();

         // Use this function to get any resource.
         // It returns a Ref to requested resource.
         // A Ref can be treated like a simple pointer.
         // You can pass it by value, dereference it, etc.
         template <typename T>
         Ref<T> Get(const char* resName);

         // This function is used internally by
         // resource managment subsystem.
         // DO NOT TRY TO USE IT TO FREE A RESOURCE!
         // The Resource will be freed automatically when
         // all Ref's pointing to it get destoyed.
         void Release(Resource* res);

      private:
         // Add Resource to hash table.
         // Checks load factor and extends hash table
         // if needed.
         void AddResource(Resource* res);

         // Insert Resource in hash table at specified
         // index. Does no checking on load factor.
         void InsertNode(unsigned int index, Resource* res);

         // Free all allocated memory for the specified resource.
         void FreeResource(const char* resName);

         // Get number of available slots in the hash table.
         // (Without chaining)
         unsigned int GetSize() const;

         // Extend hash table to next available size.
         void ExtendTable();

         // Returns index that can be used to insert
         // resource with the given Name into hash table.
         unsigned int HashResName(const char* resName);

      private:

         // This nodes are used to support chaining in the
         // hash table.
         struct ResourceNode
         {
            Resource* data;
            ResourceNode* next;
            ResourceNode(Resource* d, ResourceNode* next = nullptr)
               : data(d), next(next) {}
            ~ResourceNode();
         };

         ResourceNode** m_HashTable; // Hash table with chaining.
         int m_SizeIndex; // Use this index in size table.
         unsigned int m_ResourceCount; // Total amount of stored resources.
                                       // Used to calculate load factor.
   };

   template <typename T>
   Ref<T> ResourceManager::Get(const char* resName)
   {
      if(!resName)
         return nullptr;

      unsigned int hashIndex = HashResName(resName);
      ResourceNode *currNode = m_HashTable[hashIndex];
      while(currNode)
      { 
         if(currNode->data->GetName() && 
            0 == strcmp(currNode->data->GetName(), resName))
         {
            Resource* res = currNode->data;
            if(!res->IsLoaded())
            {
               res->Load();
               LOG_INFO("Loading resource: %s", res->GetName());
            }

            // Resource already exists, return it.
            return Ref<T>(static_cast<T*>(res));
         }
         currNode = currNode->next;
      }

      // Resource with the given Name does not already
      // exist. Create a new one and return it.
      T* newRes = new T(resName, this);
      newRes->Load();
      LOG_INFO("Loading resource: %s", newRes->GetName());
      AddResource(newRes);
      return Ref<T>(newRes);
   } 
}
#endif
