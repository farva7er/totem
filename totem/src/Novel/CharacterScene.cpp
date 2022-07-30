#include "CharacterScene.h"

namespace totem
{
   CharacterScene::CharacterNode::CharacterNode(const Character* c, int slot,
                                             CharacterNode* next)
      : m_Character(c), m_Slot(slot), m_Next(next)
   {}

   CharacterScene::CharacterScene()
      : m_Characters(nullptr)
   {}

   CharacterScene::~CharacterScene()
   {
      Clear();
   }

   void CharacterScene::Add(const Character* character, int slot)
   {
      m_Characters = new CharacterNode(character, slot, m_Characters);
   }

   void CharacterScene::Remove(const Character* character)
   {
      for(CharacterNode** curr = &m_Characters;
         *curr;
         curr = &((*curr)->m_Next))
      {
         if((*curr)->m_Character == character)
         {
            CharacterNode* tmp = *curr;
            *curr = (*curr)->m_Next;
            delete tmp;
            return;
         }
      }
   }

   void CharacterScene::Clear()
   {
      CharacterNode *curr = m_Characters, *tmp;
      while(curr)
      {
         tmp = curr;
         curr = curr->m_Next;
         delete tmp;
      }
      m_Characters = nullptr;
   }

   void CharacterScene::Draw(Renderer* renderer) const
   {
      const float characterScale = 9.0f;
      for(CharacterNode* curr = m_Characters; curr; curr = curr->m_Next)
      {
         Ref<Texture> t = curr->m_Character->GetTexture();
         if(!t)
            continue;

         float aspectRatio = t->GetWidth() / (float) t->GetHeight();
         
         Rect rect;
         rect.SetPos({ curr->m_Slot * 2.0f, 0 })
            .SetScale({ characterScale * aspectRatio, characterScale });
         renderer->DrawRect(rect, *t);
      }
   }
}
