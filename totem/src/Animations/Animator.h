#ifndef _TOTEM_ANIMATOR_H_
#define _TOTEM_ANIMATOR_H_

#include "Animation.h"

namespace totem
{
   class Animator
   {
      public:
         Animator(): m_Animations(nullptr) {}

         // There is no point in copying an Animator
         // Just construct a new one and use it.
         Animator(const Animator& other) = delete;

         ~Animator();
        
         // Same as copy constructor.
         Animator& operator=(const Animator& other) = delete;

         // Animator should be updated on each frame.
         void OnUpdate(float deltaTime);

         // Use this method to play an animation (anim).
         // Delay will cause animator to wait delay seconds
         // before starting the animation.
         // If refAnim is specified the delay will be counted
         // from the moment the refAnimation starts playing.
         void Sync(Animation* anim, float delay = 0.0f,
                   Animation* refAnim = nullptr); 
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
         };

         // Insert an animation in internal list.
         // Returns newly created node.
         AnimationNode* Insert(Animation* anim);

         // Search an animation in internal list.
         AnimationNode* SearchNode(Animation* anim) const;

      private: 
         AnimationNode* m_Animations;
   };
}
#endif
