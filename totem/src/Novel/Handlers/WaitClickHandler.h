#ifndef _TOTEM_NOVEL_HANDLERS_WAIT_CLICK_HANDLER_H_
#define _TOTEM_NOVEL_HANDLERS_WAIT_CLICK_HANDLER_H_

#include "Novel/NovelApp.h"

namespace totem
{
   class WaitClickHandler : public NovelHandler
   {
      public:

         virtual bool IsDone() const override;
         virtual void OnUpdate(float /*deltaTime*/) override {};
         virtual void OnEvent(Event& e) override;

      private:
         bool m_IsDone;
   };
}

#endif
