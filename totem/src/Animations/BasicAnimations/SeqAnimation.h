#ifndef _ANIMATIONS_BASIC_ANIMATIONS_SEQ_ANIMATION_
#define _ANIMATIONS_BASIC_ANIMATIONS_SEQ_ANIMATION_

#include "Animations/Animation.h"

namespace totem
{
   class SeqAnimation : public Animation
   {
      public:
         SeqAnimation(int start, int end, float stepDuration,
                        bool isLooping = false);
         SeqAnimation(const SeqAnimation& other) = default;

         ~SeqAnimation() = default;

         SeqAnimation& operator=(const SeqAnimation& other) = default;

         int GetCurrIndex() const;
         void SetStart(int startIndex);
         void SetEnd(int endIndex);
         void SetStep(float stepDuration);

      private:
         void RecalculateDuration();

      private:
         int m_StartIndex;
         int m_EndIndex;
         float m_StepDuration;
   };
}
#endif
