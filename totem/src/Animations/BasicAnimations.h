#ifndef _TOTEM_BASIC_ANIMATIONS_H_
#define _TOTEM_BASIC_ANIMATIONS_H_

#include "Animator.h"

namespace totem
{
   template <typename T>
   class InterpAnim : public Animation
   {
   public:
      InterpAnim(T& var, T init, T fin, float dur)
         : Animation(false, dur), 
         m_AnimVar(var), m_InitVal(init), m_FinVal(fin)
         {}
   protected:
      virtual void OnUpdate() = 0;
      virtual Animation* Clone() = 0;

      T& m_AnimVar;
      T m_InitVal;
      T m_FinVal;
   };

   template <typename T>
   class LinearInterpAnim : public InterpAnim<T>
   {
   public:
      LinearInterpAnim(T& var, T init, T fin, float dur)
         : InterpAnim<T>(var, init, fin, dur) {} 
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
   class LinearModifAnim : public InterpAnim<T>
   {
   public:
      LinearModifAnim(T& var, T fin, float dur)
         : InterpAnim<T>(var, var, fin, dur), m_StartFlag(true) {} 
   private:
      virtual void OnUpdate() override
      {
         if(m_StartFlag)
         {
            this->m_InitVal = this->m_AnimVar;
            m_StartFlag = false;
         }

         this->m_AnimVar = this->m_InitVal + 
         (this->GetCurrTime()/this->GetDuration())*
         (this->m_FinVal - this->m_InitVal);
      }

      virtual Animation* Clone() override
      {
         return new LinearModifAnim(*this);
      }

   private:
      bool m_StartFlag;
   };

   template <typename T>
   class CubicInterpAnim : public InterpAnim<T>
   {
   public:
      CubicInterpAnim(T& var, T init, T fin, float dur)
         : InterpAnim<T>(var, init, fin, dur)
      {}
      CubicInterpAnim<T>(T& var, T fin, float dur)
         : InterpAnim<T>(var, var, fin, dur)
      {}
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
   class CubicModifAnim : public InterpAnim<T>
   {
   public:
      CubicModifAnim(T& var, T fin, float dur)
         : InterpAnim<T>(var, var, fin, dur), m_StartFlag(true)
      {} 
   private:
      virtual void OnUpdate() override
      {
         if(m_StartFlag)
         {
            this->m_InitVal = this->m_AnimVar;
            m_StartFlag = false;
         }

         float t = this->GetCurrTime()/this->GetDuration();
         this->m_AnimVar = this->m_InitVal + 
                  ((t-1)*(t-1)*(t-1) + 1)*(this->m_FinVal - this->m_InitVal);
      }

      virtual Animation* Clone() override
      {
         return new CubicModifAnim(*this);
      }

   private:
      bool m_StartFlag;
   };
}


#endif
