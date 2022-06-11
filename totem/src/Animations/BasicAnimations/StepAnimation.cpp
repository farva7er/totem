#include "StepAnimation.h"
#include "Log.h"

namespace totem
{
   StepAnimation::StepAnimation(int start, int end, float stepDuration,
                                 bool isLooping)
      : Animation(isLooping, (end - start) * stepDuration),
      m_CurrIndex(start), m_StartIndex(start), m_EndIndex(end),
      m_StepDuration(stepDuration)
   {}

   StepAnimation::StepAnimation(const StepAnimation& other)
      : Animation(other),
      m_CurrIndex(other.m_StartIndex), m_StartIndex(other.m_StartIndex),
      m_EndIndex(other.m_EndIndex), m_StepDuration(other.m_StepDuration)
   {}

   void StepAnimation::SetStart(int startIndex)
   {
      m_StartIndex = startIndex;
      RecalculateDuration();
   }

   void StepAnimation::SetEnd(int endIndex)
   {
      m_EndIndex = endIndex;
      RecalculateDuration();
   }

   void StepAnimation::SetStep(float stepDuration)
   {
      m_StepDuration = stepDuration;
      RecalculateDuration();
   }

   void StepAnimation::RecalculateDuration()
   {
      SetDuration((m_EndIndex - m_StartIndex) * m_StepDuration);
   }

   void StepAnimation::OnUpdate()
   {
      m_CurrIndex = (int)(GetCurrTime() / m_StepDuration);
   }
}
