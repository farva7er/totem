#include "SeqAnimation.h"
#include "Log.h"

namespace totem
{
   SeqAnimation::SeqAnimation(int start, int end, float stepDuration,
                              bool isLooping)
      : Animation(isLooping, (end - start) * stepDuration),
         m_StartIndex(start), m_EndIndex(end), m_StepDuration(stepDuration)
   {}

   int SeqAnimation::GetCurrIndex() const
   {
      return (int)(GetCurrTime() / m_StepDuration);
   }
 
   void SeqAnimation::SetStart(int startIndex)
   {
      m_StartIndex = startIndex;
      RecalculateDuration();
   }

   void SeqAnimation::SetEnd(int endIndex)
   {
      m_EndIndex = endIndex;
      RecalculateDuration();
   }

   void SeqAnimation::SetStep(float stepDuration)
   {
      m_StepDuration = stepDuration;
      RecalculateDuration();
   }

   void SeqAnimation::RecalculateDuration()
   {
      SetDuration((m_EndIndex - m_StartIndex) * m_StepDuration);
   }
}
