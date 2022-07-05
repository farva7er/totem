#include "Character.h"
#include "NovelApp.h"
#include <string>

namespace totem
{
   Character::MoodNode::MoodNode(const char* mood, Ref<Texture> texture,
                                 MoodNode* next)
      : m_Texture(texture), m_Next(next)
   {
      if(!mood)
         mood = "Default";
      int len = strlen(mood);
      m_Mood = new char[len + 1];
      strncpy(m_Mood, mood, len);
   }

   Character::MoodNode::~MoodNode()
   {
      delete [] m_Mood;
   }

   Character::Character(const Text& name, const char* charID)
      : m_Name(name), m_NameColor(1, 1, 1, 1), m_Moods(nullptr)
   {
      if(!charID)
      {
         LOG_FATAL("charID must not be null!");
         return;
      }

      int len = strlen(charID);
      m_CharID = new char[len + 1];
      strncpy(m_CharID, charID, len + 1);

      SetMood("Default");
   }

   Character::Character(const Character& other)
   {
      m_Name = other.m_Name;

      int len = strlen(other.m_CharID);
      m_CharID = new char[len + 1];
      strncpy(m_CharID, other.m_CharID, len + 1);

      m_Moods = nullptr;
      CopyMoods(other);

      SetMood(other.m_CurrentMood->m_Mood);
   }

   Character::~Character()
   {
      delete [] m_CharID;
      DeleteMoods();
   }

   Character& Character::operator=(const Character& other)
   {
      m_Name = other.m_Name;

      int len = strlen(other.m_CharID);
      delete [] m_CharID;
      m_CharID = new char[len + 1];
      strncpy(m_CharID, other.m_CharID, len + 1);

      CopyMoods(other);
      return *this;
   }

   void Character::Say(const Text& text)
   {
      NovelApp::GetInstance()->SetSpeech(text, *this);
   }

   void Character::Show(int slot)
   {
      NovelApp::GetInstance()->ShowCharacter(*this, slot);
   }

   void Character::Hide()
   {
      NovelApp::GetInstance()->HideCharacter(*this);
   }

   void Character::SetMood(const char* mood)
   {
      MoodNode* curr = m_Moods;
      while(curr)
      {
         if(0 == strcmp(curr->m_Mood, mood))
         {
            m_CurrentMood = curr;
            return;
         }
         curr = curr->m_Next;
      }

      MoodNode* loadedMood = LoadMood(mood);
      if(!loadedMood)
      {
         LOG_ERROR("Mood Not Found %s for character %s", mood, m_CharID);
         return;
      }
      m_CurrentMood = loadedMood;
   }

   const Text& Character::GetName() const
   {
      return m_Name;
   }

   const math::vec4f& Character::GetNameColor() const
   {
      return m_NameColor;
   }

   void Character::SetNameColor(const math::vec4f& color)
   {
      m_NameColor = color;
   }

   Ref<Texture> Character::GetTexture() const
   {
      return m_CurrentMood->m_Texture;
   }

   Character::MoodNode* Character::LoadMood(const char* mood)
   {
      std::string textureFilePath = "resources/Characters/";
      textureFilePath += m_CharID;
      textureFilePath += "/";
      textureFilePath += mood;
      textureFilePath += ".png";
      Ref<Texture> texture = NovelApp::GetResourceManager()
                              ->Get<Texture>(textureFilePath.c_str());
      if(!texture)
         return nullptr;
      
      return new MoodNode(mood, texture, m_Moods);
   }

   void Character::CopyMoods(const Character& other)
   {
      DeleteMoods();
      MoodNode* curr = other.m_Moods;
      while(curr)
      {
         m_Moods = new MoodNode(curr->m_Mood, curr->m_Texture, m_Moods);
         curr = curr->m_Next;
      }
   }

   void Character::DeleteMoods()
   {
      MoodNode *curr = m_Moods, *tmp;
      while(curr)
      {
         tmp = curr;
         curr = curr->m_Next;
         delete tmp;
      }
      m_Moods = nullptr;
   } 
}
