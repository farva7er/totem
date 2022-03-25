#include "Animation.h"

namespace totem
{
   Animation::Animation(bool isLooping, float animDuration)
      : m_IsPaused(true), m_IsLooping(isLooping), m_IsAtFirstTick(true),
      m_Duration(animDuration), m_CurrTime(0), m_FinishCount(0)
      {}

   Animation::Animation(const Animation& other)
   {
      m_IsPaused = true;
      m_IsLooping = other.m_IsLooping;
      m_Duration = other.m_Duration;
      m_CurrTime = 0.0f;
      m_FinishCount = 0;
      m_IsAtFirstTick = true;
   }

   void Animation::Reset()
   {
      m_CurrTime = 0.0f;
      m_IsAtFirstTick = true;
      m_FinishCount = 0;
   }
 
   void Animation::Update(float deltaTime)
   { 
      if(m_CurrTime >= m_Duration)
      {
         m_FinishCount++;
         if(IsLooping())
            Reset();
         else
            Pause();
         return;
      }
      m_CurrTime += deltaTime;

      if(m_IsAtFirstTick)
      {
         OnStart();
         m_IsAtFirstTick = false;
      }

      OnUpdate();
   }

   AnimationGroup::AnimationGroup()
      :  m_Animations(nullptr)
   {}

   AnimationGroup::~AnimationGroup()
   {
      while(m_Animations)
      {
         AnimationNode* savedNode = m_Animations;
         m_Animations = m_Animations->next;
         delete savedNode;
      }
   }

   void AnimationGroup::Add(Animation* anim)
   {
      m_Animations = new AnimationNode(anim, m_Animations); 
   }

   void AnimationGroup::Add(const AnimationGroup& animGroup)
   {
      AnimationNode* curr = animGroup.m_Animations;
      while(curr)
      {
         Add(curr->anim);
         curr = curr->next;
      }
   }

   void AnimationGroup::Play()
   {
      AnimationNode* curr = m_Animations;
      while(curr)
      {
         curr->anim->Play();
         curr = curr->next;
      }
   }

   void AnimationGroup::Pause()
   {
      AnimationNode* curr = m_Animations;
      while(curr)
      {
         curr->anim->Pause();
         curr = curr->next;
      }
   }

   void AnimationGroup::Reset()
   {
      AnimationNode* curr = m_Animations;
      while(curr)
      {
         curr->anim->Reset();
         curr = curr->next;
      }
   }
}
