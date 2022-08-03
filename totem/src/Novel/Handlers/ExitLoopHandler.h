#ifndef _TOTEM_NOVEL_HANDLERS_EXIT_LOOP_HANDLER_H_
#define _TOTEM_NOVEL_HANDLERS_EXIT_LOOP_HANDLER_H_

#include "Novel/NovelApp.h"

namespace totem
{
   class ExitLoopHandler : public NovelHandler
   {
      public:
         virtual bool IsDone() const override { return true; }
         virtual void OnUpdate(float /*deltaTime*/) override {};
         virtual void OnEvent(Event& /*e*/) override {};
   };
}

#endif
