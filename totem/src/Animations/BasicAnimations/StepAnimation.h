#ifndef _ANIMATIONS_BASIC_ANIMATIONS_STEP_ANIMATION_
#define _ANIMATIONS_BASIC_ANIMATIONS_STEP_ANIMATION_

#include "Animations/Animation.h"

namespace totem
{
   class StepAnimation : public Animation
   {
   public:
      StepAnimation(int start, int end, float stepDuration,
                     bool isLooping = false);
      StepAnimation(const StepAnimation& other);

      int GetCurrIndex() const { return m_CurrIndex; }
      void SetStart(int startIndex);
      void SetEnd(int endIndex);
      void SetStep(float stepDuration);

      virtual void OnUpdate() override;
      virtual Animation* Clone() override
      { return new StepAnimation(*this); }
   private:
      void RecalculateDuration();

   private:
      int m_CurrIndex;
      int m_StartIndex;
      int m_EndIndex;
      float m_StepDuration;
   };
}
#endif

