#ifndef _TOTEM_ANIMATOR_H_
#define _TOTEM_ANIMATOR_H_

#include "Animation.h"

namespace totem
{
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
