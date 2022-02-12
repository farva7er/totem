#ifndef _TOTEM_ANIMATOR_H_
#define _TOTEM_ANIMATOR_H_

namespace totem
{
   class Animation
   {
      friend class Animator;

      enum class State { Play, Pause, Finish };
   public:
      Animation(bool isLooping, float animDuration);
      virtual ~Animation() {}
      void Play();
      void Pause();
      void Release();
      void Reset();
      void Finish();
      bool IsFinished() const;
      bool IsPlaying() const;
      bool IsPaused() const;
      bool IsLooping() const;
   protected:
      virtual void OnUpdate() = 0;
      float GetCurrTime() const { return m_CurrTime; }
      float GetDuration() const { return m_Duration; }
   private:
      void Update(float deltaTime);
   private:
      State m_State;
      bool m_IsLooping;
      float m_Duration;
      float m_CurrTime;
   };

   class Animator
   {
   public:
      Animator(): m_Animations(nullptr){}
      void OnUpdate(float deltaTime);
      void PlayAnim(Animation* anim);
   private:
      void Insert(Animation* anim);
   private:
      struct AnimationNode
      {
         Animation* anim;
         AnimationNode* next;
         AnimationNode(Animation* anim, AnimationNode* next = nullptr)
            : anim(anim), next(next) {}
         ~AnimationNode()
         { delete anim; }
      };
      AnimationNode* m_Animations;
   };
}
#endif
