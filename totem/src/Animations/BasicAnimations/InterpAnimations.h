#ifndef _TOTEM_INTERP_ANIMATIONS_H_
#define _TOTEM_INTERP_ANIMATIONS_H_

#include "Animations/Animation.h"

namespace totem
{
   class InterpAnim : public Animation
   {
      public:
         InterpAnim(float start, float fin, float dur)
            : Animation(false, dur), m_Start(start), m_Fin(fin)
         {}

         InterpAnim(const InterpAnim& other) = default;

         virtual ~InterpAnim() = default;

         InterpAnim& operator=(const InterpAnim& other) = default;

         virtual float GetVal() const = 0;
         void SetStart(float start) { m_Start = start; }
         void SetFin(float fin) { m_Fin = fin; }
         void SetDuration(float dur) { Animation::SetDuration(dur); }

      protected:
         float m_Start;
         float m_Fin;
   };

   class LinearInterpAnim : public InterpAnim
   {
      public:
         LinearInterpAnim(float start, float fin, float dur)
            : InterpAnim(start, fin, dur) {}

         virtual float GetVal() override
         {
            return m_Init + (GetCurrTime()/GetDuration())*
                              (m_Fin - m_Start);
         }
   };

   class CubicInterpAnim : public InterpAnim
   {
      public:
         CubicInterpAnim(float start, float fin, float dur)
            : InterpAnim(start, fin, dur) {}

         virtual float GetVal() override
         {
            float t = GetCurrTime()/GetDuration();
            return m_Init + ((t-1)*(t-1)*(t-1) + 1)*(m_Fin - m_Start);
         }
   };

   class HermiteInterpAnim : public InterpAnim
   {
      public:
         HermiteInterpAnim(float start, float fin, float dur)
            : InterpAnim(start, fin, dur) {}

         virtual float GetVal() override
         {
            float t = GetCurrTime()/GetDuration();
            return m_Init + (-2*t*t*t+3*t*t)*(m_Fin - m_Start);
         }
   };
}
#endif
