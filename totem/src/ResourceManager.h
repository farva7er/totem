#ifndef _TOTEM_RESOURCE_MANAGER_H_
#define _TOTEM_RESOURCE_MANAGER_H_

namespace totem
{
   ///////////////////////////////////////////////
   // Resources //////////////////////////////////
   ///////////////////////////////////////////////

   enum class ResourceType { Image, Audio, Font, Other };

   class Resource
   {
      public:
         Resource(const char* resId);
         virtual ~Resource() = 0;
         virtual void Load() = 0;
         virtual ResourceType GetType() const;
         const char* GetId() const { return m_ResourceId; }
      protected:
         char* m_ResourceId;
   };


   ////////////////////////////////////////////////
   // Resource Manager ( a.k.a RM )////////////////
   ////////////////////////////////////////////////

   // Specifications
   //
   // 1. There should be only one instance of ResourceManager.
   //       Therefore it should be a singleton class
   // 2. O(1) Get operation if a resource is already loaded
   //       and shouldn't be loaded second time.
   // --3. There should be possibility to operate on resources as
   //       groups and manage them based on their group


   // Basic hash table with chaining to store resources
   class ResourceManager
   {
      public:
         static ResourceManager& GetInstance();
         static void DeleteInstance();

         // returns nullptr if resource could not be found
         template <typename T>
            T* GetResource(const char* resId) const;

         template <typename T>
            T* LoadResource(T* res);

         //Remove this resource from RM table
         //Caller gets ownership of this resource
         void Untrack(const char* resId);

         // Remove resource and free all of its occupied memory
         void Unload(const char* resId);
      private:
         ResourceManager();
         ~ResourceManager();

         Resource* GetResourceInternal(const char* resId) const;
         void LoadResourceInternal(Resource* res);

         // Also checks load factor and extends table if needed
         void Insert(Resource *res);
         // Does no checking on load factor
         void InsertNode(unsigned int index, Resource* res);

         unsigned int GetSize() const;

         void ExtendTable();
         unsigned int HashString(const char* str) const;
      private:

         struct ResourceNode
         {
            Resource* data;
            ResourceNode* next;
            ResourceNode(Resource* d, ResourceNode* next = nullptr)
               : data(d), next(next) {}
            ~ResourceNode();
         };

         static ResourceManager* s_Instance;
         ResourceNode** m_Table;
         int m_SizeIndex;
         unsigned int m_ResourceCount;
   };

   template <typename T>
      T* ResourceManager::GetResource(const char* resId) const
      {
         Resource* res = GetResourceInternal(resId);
         TOTEM_ASSERT(!res || T::GetStaticType() == res->GetType(), 
               "Resource Types does not match for %s");
         return reinterpret_cast<T*>(res);
      }

   template <typename T>
      T* ResourceManager::LoadResource(T* res)
      {
         LoadResourceInternal(res);
         TOTEM_ASSERT(T::GetStaticType() == res->GetType(),
               "Resource Types does not match for %s");
         return res;
      }
}
#endif
