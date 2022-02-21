#ifndef _TOTEM_ANIMATOR_H_
#define _TOTEM_ANIMATOR_H_

namespace totem
{
   class Animation
   {
      friend class Animator;

      enum class State { Play, PauseFromPlay, Delay, PauseFromDelay };
   public:
      Animation(bool isLooping, float animDuration);
      Animation(const Animation& other);

      virtual ~Animation() {}
   private:
      void Play();
      void Pause();
      void Delay();
      void Reset();
   protected:
      int GetFinishCount() const;
      bool HasFinished() const;
      bool IsDelayed() const;
      bool IsPlaying() const;
      bool IsPaused() const;
      bool IsLooping() const;
      bool IsAtStart() const;

   public:
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
      bool m_StartFlag;
   };

   class AnimationGroup
   {
      friend class Animator;

   public:
      AnimationGroup();
      ~AnimationGroup();
      void Add(Animation* anim);
      void Add(const AnimationGroup& animGroup);
   private:
      struct AnimationNode
      {
         Animation* anim;
         AnimationNode* next;
         AnimationNode(Animation* anim, AnimationNode *next = nullptr)
            : anim(anim), next(next) {}
      };

      AnimationNode* m_Animations;
   };

   class Animator
   {
   public:
      Animator(): m_Animations(nullptr){}
      ~Animator();
      void OnUpdate(float deltaTime);
      void Add(Animation* anim);
      void Play( Animation* anim, 
                     float delay = 0.0f, Animation* refAnim = nullptr);
      void Pause(Animation* anim);
      void Release(Animation *anim);

      void Add(const AnimationGroup& animGroup);
      void Play(const AnimationGroup& animGroup, 
                     float delay = 0.0f, Animation* refAnim = nullptr);
      void Pause(const AnimationGroup& animGroup);
      void Release(const AnimationGroup& animGroup);
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
