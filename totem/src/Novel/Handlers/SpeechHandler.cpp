#include "SpeechHandler.h"

namespace totem
{
   SpeechHandler::SpeechHandler(DialogBox* dialogBox)
      : m_IsDone(false), m_DialogBox(dialogBox)
   {}

   bool SpeechHandler::IsDone() const
   {
      return m_IsDone;
   }

   void SpeechHandler::OnEvent(Event& e)
   {
      if(e.GetType() == MousePressedEvent::GetStaticType())
      {
         MousePressedEvent& mpe = e.Cast<MousePressedEvent>();
         if(mpe.GetButton() == 0) // Left Mouse Button
         {
            if(m_DialogBox->IsAnimationPlaying())
            {
               m_DialogBox->SkipAnimation();
            }
            else
            { 
               m_IsDone = true;
            }
         }
      }
   }
}
