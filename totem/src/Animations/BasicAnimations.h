#ifndef _TOTEM_BASIC_ANIMATIONS_H_
#define _TOTEM_BASIC_ANIMATIONS_H_

#include "Animator.h"

namespace totem
{
   template <typename T>
   class LinearAnim : public Animation
   {
   public:
      LinearAnim(T& var, T init, T fin, float dur)
         : Animation(false, dur), 
         m_AnimVar(var), m_InitVal(init), m_FinVal(fin)
         {}
   private:
      void OnUpdate()
      {
         m_AnimVar = m_InitVal + 
                     (GetCurrTime()/GetDuration())*(m_FinVal - m_InitVal);
      }
   private:
      T& m_AnimVar;
      T m_InitVal;
      T m_FinVal;
   };
}



#endif
