#include "IUIElement.h"

namespace totem
{
   unsigned int IUIElement::s_ID_Counter = 0;

   unsigned int IUIElement::ObtainID()
   {
      return ++s_ID_Counter;
   }
}
