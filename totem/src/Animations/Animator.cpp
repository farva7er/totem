#include "Animator.h"
#include "Log.h"

namespace totem
{
   Animation::Animation(bool isLooping, float animDuration)
      : m_State(State::Pause), m_IsLooping(isLooping),
      m_Duration(animDuration), m_CurrTime(0), m_FinishCount(0) {}

   Animation::Animation(const Animation& other)
   {
      m_State = State::Pause;
      m_IsLooping = other.m_IsLooping;
      m_Duration = other.m_Duration;
      m_CurrTime = 0.0f;
      m_FinishCount = 0;
   }

   void Animation::Play()
   {
      m_State = State::Play;
   }

   void Animation::Pause()
   {
      m_State = State::Pause;
   }

   void Animation::Delay()
   {
      m_State = State::Delay;
   }

   void Animation::Reset()
   {
      m_CurrTime = 0.0f;
   }

   bool Animation::IsDelayed() const
   {
      return m_State == State::Delay;
   }

   int Animation::GetFinishCount() const
   {
      return m_FinishCount;
   }

   bool Animation::IsPlaying() const
   {
      return m_State == State::Play;
   }

   bool Animation::IsPaused() const
   {
      return m_State == State::Pause;
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
         OnUpdate();
         //LOG_INFO("Anim Update %f", deltaTime);
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
          
         if(currAnim->IsDelayed())
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

   void Animator::AddAnim(Animation* anim)
   {
      Insert(anim, 0.0f, nullptr);
   }

   void Animator::PlayAnim(Animation* anim, float delay, Animation* refAnim)
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

      anim->Delay(); 
   }

   void Animator::Insert(Animation* anim, float delay, Animation* refAnim)
   {
      AnimationNode* node = 
         new AnimationNode(anim, refAnim, delay, m_Animations);
      m_Animations = node; 
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
};
   

