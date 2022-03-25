#include "Animator.h"
#include "Log.h"
namespace totem
{

/////////////////////////////////////////////////
// Animator /////////////////////////////////////
/////////////////////////////////////////////////

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

   void Animator::Add(Animation* anim)
   {
      AnimationNode* animNode = SearchNode(anim);
      if(!animNode)
      {
         Insert(anim, 0.0f, nullptr);
      }
   }

   void Animator::Add(const AnimationGroup& animGroup)
   {
      AnimationGroup::AnimationNode* curr = animGroup.m_Animations;
      while(curr)
      {
         Add(curr->anim);
         curr = curr->next;
      }
   }

   void Animator::Sync(Animation* anim, float delay, Animation* refAnim)
   {
      AnimationNode* animNode = SearchNode(anim);
      if(!animNode)
      {
         Insert(anim, delay, refAnim);
         animNode = SearchNode(anim);
      }

      animNode->delay = delay;
      animNode->refAnim = refAnim;

      anim->Play();
   }

   void Animator::Insert(Animation* anim, float delay, Animation* refAnim)
   {
      m_Animations = new AnimationNode(anim, refAnim, delay, m_Animations);
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

   void Animator::Sync(const AnimationGroup& animGroup, float delay,
                       Animation* refAnim)
   {
      AnimationGroup::AnimationNode* curr = animGroup.m_Animations;
      while(curr)
      {
         Sync(curr->anim, delay, refAnim);
         curr = curr->next;
      }
   }
};
