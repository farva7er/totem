#include "UIElement.h"
#include "UIManager.h"

namespace totem
{
   UIElement::UIElement(UIManager* manager)
      : m_Master(manager)
   {}

   UIElement::~UIElement()
   {
      m_Master->Forget(this);
   }

   Renderer* UIElement::GetRenderer() const
   {
      return m_Master->GetRenderer();
   }
}
   
