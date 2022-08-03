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

         virtual ~Animation() = default;

         Animation& operator=(const Animation& other);

         void Play() { m_IsPaused = false; }
         void Pause() { m_IsPaused = true; }
         void Reset();

         float GetCurrTime() const { return m_CurrTime; }
         float GetDuration() const { return m_Duration; }

         bool IsPaused() const { return m_IsPaused; }

         int GetFinishCount() const { return m_FinishCount; }
         bool HasFinishedOnce() const { return m_FinishCount > 0; }
         bool IsLooping() const { return m_IsLooping; }

      protected:
         virtual void OnUpdate() {};
         virtual void OnStart() {}

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
}

#endif
