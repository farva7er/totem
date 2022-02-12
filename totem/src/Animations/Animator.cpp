#include "Animator.h"
#include "Log.h"

namespace totem
{
   Animation::Animation(bool isLooping, float animDuration)
      : m_State(State::Start), m_IsLooping(isLooping),
      m_Duration(animDuration), m_CurrTime(0) {}

   Animation::Animation(const Animation& other)
   {
      m_State = State::Start;
      m_IsLooping = other.m_IsLooping;
      m_Duration = other.m_Duration;
      m_CurrTime = 0.0f;
   }

   void Animation::Play()
   {
      m_State = State::Play;
   }

   void Animation::Pause()
   {
      m_State = State::Pause;
   }

   void Animation::Release()
   {
      m_State = State::Play;
   }

   void Animation::Finish()
   {
      m_State = State::Finish;
   }

   void Animation::Reset()
   {
      m_CurrTime = 0.0f;
   }
 
   bool Animation::IsAtStart() const
   {
      return m_State == State::Start;
   }

   bool Animation::IsFinished() const
   {
      return m_State == State::Finish;
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
            if(IsLooping())
               Reset();
            else
               Finish();
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
         if(currAnim->IsFinished() && (*currNode)->refCount <= 0)
         {
            AnimationNode* savedNode = (*currNode);
            (*currNode) = (*currNode)->next;
            delete savedNode;
            LOG_INFO("Anim Deleted");
            continue;
         }
         
         if(currAnim->IsAtStart())
         {
            AnimationNode* refNode = (*currNode)->refNode;
            if(refNode && refNode->anim->IsFinished() &&
                        (*currNode)->delay <= 0.0f)
            {
               currAnim->Play();
               refNode->refCount--; 
            }
            if(!refNode && (*currNode)->delay <= 0.0f)
            {
               currAnim->Play();
            }
            if(refNode && refNode->anim->IsFinished())
            {
               (*currNode)->delay-=deltaTime;
            }
            if(!refNode)
            { 
               (*currNode)->delay-=deltaTime;
            }
         }
            
         currAnim->Update(deltaTime);
         currNode = &((*currNode)->next);
      }
   }

   void Animator::PlayAnim(Animation* anim, float delay, Animation* refAnim)
   {
      AnimationNode* refNode = nullptr;
      if(refAnim)
         refNode = SearchNode(refAnim);
      Insert(anim, delay, refNode);
   }

   void Animator::Insert(Animation* anim, float delay, AnimationNode *refNode)
   {
      AnimationNode* node = 
         new AnimationNode(anim, refNode, delay, m_Animations);
      m_Animations = node;
      if(refNode)
         refNode->refCount++; 
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
   

