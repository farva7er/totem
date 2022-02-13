#ifndef _TOTEM_ANIMATOR_H_
#define _TOTEM_ANIMATOR_H_

namespace totem
{
   class Animation
   {
      friend class Animator;

      enum class State { Play, Pause, Delay };
   public:
      Animation(bool isLooping, float animDuration);
      Animation(const Animation& other);

      virtual ~Animation() {}
      void Play();
      void Pause();
      void Delay();
      void Reset();
      bool IsDelayed() const;
      int GetFinishCount() const;
      bool IsPlaying() const;
      bool IsPaused() const;
      bool IsLooping() const;

      virtual Animation* Clone() = 0;

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
      int m_FinishCount;
   };

   class Animator
   {
   public:
      Animator(): m_Animations(nullptr){}
      ~Animator();
      void OnUpdate(float deltaTime);
      void AddAnim(Animation* anim);
      void PlayAnim( Animation* anim, 
                     float delay = 0.0f, Animation* refAnim = nullptr);
   private:
      struct AnimationNode
      {
         Animation* anim;
         Animation* refAnim;
         float delay;
         AnimationNode* next;
         AnimationNode(Animation* anim, Animation* refAnim,
                        float delay, AnimationNode* next = nullptr)
            : anim(anim), refAnim(refAnim),
            delay(delay), next(next) {}
         ~AnimationNode() { delete anim; }
      };

      void Insert(Animation* anim, float delay, Animation* refAnim);
      AnimationNode* SearchNode(Animation* anim) const;
   private:
      AnimationNode* m_Animations;
   };
}
#endif
