#ifndef _TOTEM_NOVEL_HANDLERS_IDLE_HANDLER_
#define _TOTEM_NOVEL_HANDLERS_IDLE_HANDLER_

namespace totem
{
   class IdleHandler : public NovelHandler
   {
      public:
         virtual bool IsDone() const override { return false; }
         virtual void OnUpdate(float /*deltaTime*/) override {};
         virtual void OnEvent(Event& /*e*/) override {};
   };
}

#endif
