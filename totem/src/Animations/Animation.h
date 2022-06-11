#ifndef _TOTEM_ANIMATION_H_
#define _TOTEM_ANIMATION_H_

namespace totem
{
   class Animation
   {
      friend class Animator;

   public:
      Animation(bool isLooping, float animDuration);
      Animation(const Animation& other);

      virtual ~Animation() {}
      virtual Animation* Clone() = 0;

      void Play() { m_IsPaused = false; };
      void Pause() { m_IsPaused = true; };
      void Reset();

      bool IsPaused() { return m_IsPaused; }

   protected:
      int GetFinishCount() const { return m_FinishCount; };
      bool HasFinishedOnce() const { return m_FinishCount > 0; };
      bool IsLooping() const { return m_IsLooping; };

   protected:
      virtual void OnUpdate() = 0;
      virtual void OnStart() {}
      float GetCurrTime() const { return m_CurrTime; }
      float GetDuration() const { return m_Duration; }

      void SetDuration(float dur) { m_Duration = dur; }

   private:
      void Update(float deltaTime);
   private:
      bool m_IsPaused;
      bool m_IsLooping;
      bool m_IsAtFirstTick;
      float m_Duration;
      float m_CurrTime;
      int m_FinishCount;
   };

   class AnimationGroup
   {
      friend class Animator;

   public:
      AnimationGroup();
      ~AnimationGroup();
      void Add(Animation* anim);
      void Add(const AnimationGroup& animGroup);

      void Play();
      void Pause();
      void Reset();

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
}

#endif
