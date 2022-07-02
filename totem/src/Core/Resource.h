#ifndef _TOTEM_RESOURCE_H_
#define _TOTEM_RESOURCE_H_

namespace totem
{
   class ResourceManager;

   class Resource
   {
      public:
         Resource(const char* name, ResourceManager* manager);
         virtual ~Resource();

         // Shouldn't be copyable since it's ref counted.
         Resource(const Resource& other) = delete;
         Resource& operator=(const Resource& other) = delete;

         void Release();

         virtual void Load() = 0;
         void SetLoaded() { m_IsLoaded = true; }
         bool IsLoaded() const { return m_IsLoaded; }

         const char* GetName() const { return m_Name; }

         int GetRef() { return m_RefCount; }
         void IncRef() { m_RefCount++; }
         void DecRef() { m_RefCount--; }

      private:
         ResourceManager* m_Manager;
         char* m_Name;
         int m_RefCount;
         bool m_IsLoaded;
   };

   template <typename ResourceType>
   class Ref
   {
      public:
         Ref() : m_Resource(nullptr) {}

         Ref(ResourceType* resource)
            : m_Resource(resource)
         {
            if(resource)
               m_Resource->IncRef();
         }

         Ref(const Ref& other)
            : m_Resource(other.m_Resource)
         {
            if(m_Resource)
               m_Resource->IncRef();
         }

         Ref<ResourceType>& operator=(const Ref& other)
         {
            if(m_Resource)
               m_Resource->Release();
            m_Resource = other.m_Resource;
            if(m_Resource)
               m_Resource->IncRef();
            return *this;
         }

         ~Ref()
         {
            if(m_Resource)
               m_Resource->Release();
         }

         ResourceType& operator*() const
         {
            return *m_Resource;
         }

         ResourceType* operator->() const
         {
            return m_Resource;
         }

         operator bool() const
         {
            return m_Resource != nullptr;
         }

      private:
         ResourceType* m_Resource;
   };

}

#endif
