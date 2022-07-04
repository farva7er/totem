#include "Animation.h"

namespace totem
{
   Animation::Animation(bool isLooping, float animDuration)
      : m_IsPaused(true), m_IsLooping(isLooping), m_IsAtFirstTick(true),
      m_Duration(animDuration), m_CurrTime(0.0f), m_FinishCount(0)
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

   Animation& Animation::operator=(const Animation& other)
   {
      m_IsPaused = true;
      m_IsLooping = other.m_IsLooping;
      m_Duration = other.m_Duration;
      m_CurrTime = 0.0f;
      m_FinishCount = 0;
      m_IsAtFirstTick = true;
      return *this;
   }

   void Animation::Reset()
   {
      m_CurrTime = 0.0f;
      m_IsAtFirstTick = true;
      m_FinishCount = 0;
   }
 
   void Animation::Update(float deltaTime)
   { 
      if(m_IsAtFirstTick)
      {
         OnStart();
         m_IsAtFirstTick = false;
      }

      OnUpdate();

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
   }
}
