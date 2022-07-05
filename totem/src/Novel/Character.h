#ifndef _TOTEM_NOVEL_CHARACTER_H_
#define _TOTEM_NOVEL_CHARACTER_H_

#include "Internationalization/Text.h"
#include "Core/ResourceManager.h"
#include "Rendering/Texture.h"
#include "Math/Vec.h"

namespace totem
{
   class Character
   {
      public:
         Character(const Text& name, const char* charID);
         Character(const Character& other);

         ~Character();

         Character& operator=(const Character& other);

         void Say(const Text& text);
         void Show(int slot);
         void Hide();

         void SetMood(const char* mood);
         const Text& GetName() const;
         const math::vec4f& GetNameColor() const;
         void SetNameColor(const math::vec4f& color);

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
         math::vec4f m_NameColor;
 
         MoodNode* m_Moods;
         MoodNode* m_CurrentMood;
   };
}

#endif
