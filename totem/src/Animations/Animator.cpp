#include "Animator.h"
#include "Log.h"

namespace totem
{
   Animator::~Animator()
   {
      AnimationNode* currNode = m_Animations;
      while(currNode)
      {
         AnimationNode* savedNode = currNode;
         currNode = currNode->next;
         delete savedNode;
      }
   }

   void Animator::OnUpdate(float deltaTime)
   {
      AnimationNode* currNode = m_Animations;
      while(currNode)
      {
         Animation* currAnim = currNode->anim;
         
         if(!currAnim->IsPaused())
         {
            if(currNode->delay > 0)
            {
               if(currNode->refAnim && currNode->refAnim->HasFinishedOnce())
               {
                  currNode->delay -= deltaTime;
               }
               else if(!currNode->refAnim)
               {
                  currNode->delay -= deltaTime;
               }
            }
            else
            {
               currAnim->Update(deltaTime);
            }      
         }

         currNode = currNode->next;
      }
   }
 
   void Animator::Sync(Animation* anim, float delay, Animation* refAnim)
   {
      AnimationNode* animNode = SearchNode(anim);
      if(!animNode)
      {
         animNode = Insert(anim);
      }

      animNode->delay = delay;
      animNode->refAnim = refAnim;

      anim->Play();
   }

   Animator::AnimationNode* Animator::Insert(Animation* anim)
   {
      m_Animations = new AnimationNode(anim, nullptr, 0.0f, m_Animations);
      return m_Animations;
   }

   Animator::AnimationNode* Animator::SearchNode(Animation* anim) const
   {
      AnimationNode *currNode = m_Animations;
      while(currNode)
      {
         if(currNode->anim == anim)
            return currNode;
         currNode = currNode->next;
      }
      return nullptr;
   }
}
