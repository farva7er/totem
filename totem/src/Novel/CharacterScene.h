#ifndef _TOTEM_NOVEL_CHARACTER_SCENE_H_
#define _TOTEM_NOVEL_CHARACTER_SCENE_H_

#include "Character.h"
#include "Rendering/Renderer.h"

namespace totem
{
   class CharacterScene
   {
      public:
         CharacterScene();
         ~CharacterScene();

         void Add(const Character* character, int slot);
         void Remove(const Character* character);

         void Draw(Renderer* renderer) const;

      private:
         struct CharacterNode
         {
            const Character* m_Character;
            int m_Slot;
            CharacterNode* m_Next;
            CharacterNode(const Character* c, int slot,
                           CharacterNode* next = nullptr);
         };

         CharacterNode* m_Characters;
   };
}

#endif
