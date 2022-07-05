#ifndef _TOTEM_NOVEL_CHARACTER_H_
#define _TOTEM_NOVEL_CHARACTER_H_

#include "Internationalization/Text.h"
#include "Core/ResourceManager.h"
#include "Rendering/Texture.h"

namespace totem
{
   class Character
   {
      public:
         Character(const Text& name, const char* charID);
         Character(const Character& other);

         ~Character();

         Character& operator=(const Character& other);

         void Show(int slot);
         void Hide();

         void SetMood(const char* mood);

         Ref<Texture> GetTexture() const;

      private:
         struct MoodNode
         {
            char* m_Mood;
            Ref<Texture> m_Texture;
            MoodNode* m_Next;
            MoodNode(const char* mood, Ref<Texture> texture,
                     MoodNode* next = nullptr);
            ~MoodNode();
         };

         MoodNode* LoadMood(const char* mood);
         void CopyMoods(const Character& other);
         void DeleteMoods();

      private:
         Text m_Name;
         char* m_CharID;
 
         MoodNode* m_Moods;
         MoodNode* m_CurrentMood;
   };
}

#endif
