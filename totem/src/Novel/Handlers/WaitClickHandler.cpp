#include "WaitClickHandler.h"

namespace totem
{
   bool WaitClickHandler::IsDone() const
   {
      return m_IsDone;
   }

   void WaitClickHandler::OnEvent(Event& e) 
   {
      if(e.IsHandled())
         return;

      if(e.GetType() == MousePressedEvent::GetStaticType())
      {
         MousePressedEvent& mpe = e.Cast<MousePressedEvent>();
         if(mpe.GetButton() == 0) // Left Mouse Button
         {
            m_IsDone = true;
         }
      }
   }
}
