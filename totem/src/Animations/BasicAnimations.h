#ifndef _TOTEM_BASIC_ANIMATIONS_H_
#define _TOTEM_BASIC_ANIMATIONS_H_

#include "Animator.h"
#include "Log.h"

namespace totem
{
   template <typename T>
   class InterpAnim : public Animation
   {
   public:
      InterpAnim(T init, T fin, float dur)
         : Animation(false, dur), 
         m_AnimVar(init), m_InitVal(init), m_FinVal(fin)
      {}

      void ApplyVal(T& var)
      {
         if(!this->IsPaused())
         {
            var = this->m_AnimVar;
         }
      }

      void SetInitVal(T init)
      {
         this->m_InitVal = init;
      }

      void SetFinVal(T fin)
      {
         this->m_FinVal = fin;
      }

   protected:

      virtual void OnUpdate() = 0;
      virtual Animation* Clone() = 0;

      T m_AnimVar;
      T m_InitVal;
      T m_FinVal;
   };

   template <typename T>
   class LinearInterpAnim : public InterpAnim<T>
   {
   public:
      LinearInterpAnim(T init, T fin, float dur)
         : InterpAnim<T>(init, fin, dur) {} 
   private:
      virtual void OnUpdate() override
      {
         this->m_AnimVar = this->m_InitVal + 
         (this->GetCurrTime()/this->GetDuration())*
         (this->m_FinVal - this->m_InitVal);
      }

      virtual Animation* Clone() override
      {
         return new LinearInterpAnim(*this);
      }
   };

   template <typename T>
   class CubicInterpAnim : public InterpAnim<T>
   {
   public:
      CubicInterpAnim(T init, T fin, float dur)
         : InterpAnim<T>(init, fin, dur) {} 
   private:
      virtual void OnUpdate() override
      {
         float t = this->GetCurrTime()/this->GetDuration();
         this->m_AnimVar = this->m_InitVal + 
                  ((t-1)*(t-1)*(t-1) + 1)*(this->m_FinVal - this->m_InitVal);
      }

      virtual Animation* Clone() override
      {
         return new CubicInterpAnim(*this);
      }
   };

   template <typename T>
   class HermiteInterpAnim : public InterpAnim<T>
   {
   public:
      HermiteInterpAnim(T init, T fin, float dur)
         : InterpAnim<T>(init, fin, dur) {}
   private:
      virtual void OnUpdate() override
      {
         float t = this->GetCurrTime()/this->GetDuration();
         this->m_AnimVar = this->m_InitVal + 
                  (-2*t*t*t+3*t*t)*(this->m_FinVal - this->m_InitVal);
      }

      virtual Animation* Clone() override
      {
         return new HermiteInterpAnim(*this);
      }
   };
}
#endif
