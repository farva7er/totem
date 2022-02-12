#include "Animator.h"
#include "Log.h"

namespace totem
{
   Animation::Animation(bool isLooping, float animDuration)
      : m_State(State::Pause), m_IsLooping(isLooping),
      m_Duration(animDuration), m_CurrTime(0) {}

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
         LOG_INFO("Anim Update %f", deltaTime);
      } 
   }


/////////////////////////////////////////////////
// Animator /////////////////////////////////////
/////////////////////////////////////////////////

   void Animator::OnUpdate(float deltaTime)
   {
      AnimationNode** currNode = &m_Animations;
      while(*currNode)
      {
         Animation* currAnim = (*currNode)->anim;
         if(currAnim->IsFinished())
         {
            AnimationNode* savedNode = (*currNode);
            (*currNode) = (*currNode)->next;
            delete savedNode;
            LOG_INFO("Anim Deleted");
            continue;
         }
            
         currAnim->Update(deltaTime);
         currNode = &((*currNode)->next);
      }
   }

   void Animator::PlayAnim(Animation* anim)
   {
      Insert(anim);
      anim->Play();
   }

   void Animator::Insert(Animation* anim)
   {
      AnimationNode* node = new AnimationNode(anim, m_Animations);
      m_Animations = node;
   }
};
   

