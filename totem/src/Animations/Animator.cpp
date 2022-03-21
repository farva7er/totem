#include "Animator.h"
#include "Log.h"
namespace totem
{
   Animation::Animation(bool isLooping, float animDuration)
      : m_State(State::PauseFromPlay), m_IsLooping(isLooping), m_IsAtFirstTick(true),
      m_Duration(animDuration), m_CurrTime(0), m_FinishCount(0)
      {}

   Animation::Animation(const Animation& other)
   {
      m_State = State::PauseFromPlay;
      m_IsLooping = other.m_IsLooping;
      m_Duration = other.m_Duration;
      m_CurrTime = 0.0f;
      m_FinishCount = 0;
      m_IsAtFirstTick = true;
   }

   void Animation::Play()
   {
      m_State = State::Play;
   }

   void Animation::Pause()
   {
      if(m_State == State::Play)
         m_State = State::PauseFromPlay;
      else if(m_State == State::Delay)
         m_State = State::PauseFromDelay;
   }
   void Animation::Delay()
   {
      m_State = State::Delay;
   }

   void Animation::Reset()
   {
      m_CurrTime = 0.0f;
      m_IsAtFirstTick = true;
      m_FinishCount = 0;
   }

   bool Animation::IsDelayed() const
   {
      return m_State == State::Delay || m_State == State::PauseFromDelay;
   }

   int Animation::GetFinishCount() const
   {
      return m_FinishCount;
   }

   bool Animation::HasFinished() const
   {
      return m_FinishCount > 0;
   }

   bool Animation::IsPlaying() const
   {
      return m_State == State::Play;
   }

   bool Animation::IsPaused() const
   {
      return m_State == State::PauseFromPlay ||
            m_State == State::PauseFromDelay;
   }
   
   bool Animation::IsLooping() const
   {
      return m_IsLooping;
   }

   void Animation::Update(float deltaTime)
   { 
      if(IsPlaying())
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
            OnStart();

         m_IsAtFirstTick = false;
         OnUpdate();
      } 
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
      AnimationNode** currNode = &m_Animations;
      while(*currNode)
      {
         Animation* currAnim = (*currNode)->anim;
          
         if(currAnim->IsDelayed() && !currAnim->IsPaused())
         {
            Animation* refAnim = (*currNode)->refAnim;
            if(refAnim && refAnim->GetFinishCount() > 0 &&
                        (*currNode)->delay <= 0.0f)
            {
               currAnim->Play();
            }
            if(!refAnim && (*currNode)->delay <= 0.0f)
            {
               currAnim->Play();
            }
            if(refAnim && refAnim->GetFinishCount() > 0)
            {
               (*currNode)->delay-=deltaTime;
            }
            if(!refAnim)
            { 
               (*currNode)->delay-=deltaTime;
            }
         }
            
         currAnim->Update(deltaTime);
         currNode = &((*currNode)->next);
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

   void Animator::Play(Animation* anim, float delay, Animation* refAnim)
   {
      AnimationNode* animNode = SearchNode(anim);
      if(!animNode)
      {
         Insert(anim, delay, refAnim);
      }
      else
      {
         animNode->delay = delay;
         animNode->refAnim = refAnim;
      }

      anim->Reset();
      anim->Delay(); 
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

   void Animator::Pause(Animation* anim)
   {
      anim->Pause();
   }

   void Animator::Release(Animation* anim)
   {
      if(!anim->IsDelayed() && anim->IsPaused())
      {
         if(anim->IsLooping() || !anim->HasFinished())
            anim->Play();
      }
      else if(anim->IsDelayed() && anim->IsPaused())
      {
         anim->Delay();
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

   void Animator::Play( const AnimationGroup& animGroup, float delay,
                        Animation* refAnim)
   {
      AnimationGroup::AnimationNode* curr = animGroup.m_Animations;
      while(curr)
      {
         Play(curr->anim, delay, refAnim);
         curr = curr->next;
      }
   }

   void Animator::Pause(const AnimationGroup& animGroup)
   {
      AnimationGroup::AnimationNode* curr = animGroup.m_Animations;
      while(curr)
      {
         Pause(curr->anim);
         curr = curr->next;
      }
   }

   void Animator::Release(const AnimationGroup& animGroup)
   {
      AnimationGroup::AnimationNode* curr = animGroup.m_Animations;
      while(curr)
      {
         Release(curr->anim);
         curr = curr->next;
      }
   }
};
