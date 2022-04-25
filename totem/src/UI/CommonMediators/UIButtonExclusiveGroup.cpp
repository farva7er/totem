#include "UIButtonExclusiveGroup.h"

namespace totem
{
   UIButtonExclusiveGroup::~UIButtonExclusiveGroup()
   {
      while(m_Buttons)
      {
         IButtonNode* savedNode = m_Buttons;
         m_Buttons = m_Buttons->m_Next;
         delete savedNode;
      }
   }

   void UIButtonExclusiveGroup::AddButton(CheckButton* button)
   {
      ButtonListener* listener = new ButtonListener(this, button->GetID());
      m_Buttons = new IButtonNode(button, listener, m_Buttons);
      button->AddListener(listener);
   }

   void UIButtonExclusiveGroup::RemoveButton(CheckButton* button)
   {
      IButtonNode** curr = &m_Buttons;
      while(*curr)
      {
         if((*curr)->m_Button->GetID() == button->GetID())
         {
            IButtonNode* savedNode = *curr;
            *curr = (*curr)->m_Next;
            delete savedNode;
         }
         curr = &((*curr)->m_Next);
      }
   }

   void UIButtonExclusiveGroup::OnButtonClicked(unsigned int buttonID)
   {
      IButtonNode* curr = m_Buttons;
      while(curr)
      {
         if(curr->m_Button->GetID() == buttonID)
         {
            curr->m_Button->SetActive(false);
         }
         else
         {
            curr->m_Button->SetActive(true);
            curr->m_Button->SetChecked(false);
         }

         curr = curr->m_Next;
      }
   }

   UIButtonExclusiveGroup::IButtonNode::~IButtonNode()
   {
      m_Button->RemoveListener(m_Listener);
      delete m_Listener;
   }
}
