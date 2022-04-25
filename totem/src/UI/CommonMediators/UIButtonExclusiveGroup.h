#ifndef _TOTEM_UI_BUTTON_EXCLUSIVE_GROUP_H_
#define _TOTEM_UI_BUTTON_EXCLUSIVE_GROUP_H_

#include "UI/CheckButton.h"
#include "UI/IIEListener.h"

// This class does not own buttons,
// it just helps them to interact

namespace totem
{
   class UIButtonExclusiveGroup
   {
   public:
      UIButtonExclusiveGroup()
         : m_Buttons(nullptr)
      {}

      ~UIButtonExclusiveGroup();

      // Temporary (or maybe not..)
      UIButtonExclusiveGroup(const UIButtonExclusiveGroup& other) = delete;

      // Temporary (or maybe not..)
      UIButtonExclusiveGroup&
      operator=(const UIButtonExclusiveGroup& other) = delete;

      void AddButton(CheckButton* button);
      void RemoveButton(CheckButton* button);
      void OnButtonClicked(unsigned int buttonId);

   private:
      class ButtonListener : public IIEListener
      {
      public:
         ButtonListener(UIButtonExclusiveGroup* master, unsigned int buttonID)
            : m_Master(master), m_ButtonID(buttonID) {}

      private:
         virtual void OnClick(int /*button*/) override
         {
            m_Master->OnButtonClicked(m_ButtonID);
         }
    
         virtual totem::IIEListener* Clone() const
         {
            return new ButtonListener(m_Master, m_ButtonID);
         }
         
         UIButtonExclusiveGroup* m_Master;
         unsigned int m_ButtonID;
      };

      struct IButtonNode
      {
         CheckButton* m_Button;
         ButtonListener* m_Listener;
         IButtonNode* m_Next;
         IButtonNode(CheckButton* button, ButtonListener* listener,
                     IButtonNode* next = nullptr)
            : m_Button(button), m_Listener(listener), m_Next(next)
         {}
         ~IButtonNode();
      };

      IButtonNode* m_Buttons;
   };
}

#endif

