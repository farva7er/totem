#ifndef _TOTEM_NOVEL_HANDLERS_SPEECH_HANDLER_H_
#define _TOTEM_NOVEL_HANDLERS_SPEECH_HANDLER_H_

#include "Novel/NovelApp.h"

namespace totem
{
   /* This handler provides skipping DialogBox text animation.
    * If player clicks during animation, the animation is
    * skipped and full text is shown immediately.
    * If player clicks when animation is finished, the
    * handler indicates that it's done and the next
    * handler should be fetched.
    */

   class SpeechHandler : public NovelHandler
   {
      public:
         SpeechHandler(DialogBox* dialogBox);

         virtual bool IsDone() const override;
         virtual void OnUpdate(float /*deltaTime*/) override {};
         virtual void OnEvent(Event& e) override;

      private:
         bool m_IsDone;
         DialogBox* m_DialogBox;
   };
}
#endif
