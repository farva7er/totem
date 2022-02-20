#include "UIManager.h"
#include "UIElement.h"

namespace totem
{
   UIManager::UIElementNode::~UIElementNode()
   {
      delete element;
   }

   UIManager::UIManager(Renderer* renderer)
      : m_Elements(nullptr), m_Renderer(renderer)
   {}

   UIManager::~UIManager()
   {
      UIElementNode* curr = m_Elements;
      while(curr)
      {
         UIElementNode* savedNode = curr;
         UIElement* savedElement = curr->element;
         savedNode->element = nullptr;
         delete savedElement;
         delete savedNode;
         curr = curr->next;
      }
   }

   void UIManager::AddElement(UIElement* element)
   {
      m_Elements = new UIElementNode(element, m_Elements);
   }

   void UIManager::OnUpdate(float deltaTime)
   {
      UIElementNode* curr = m_Elements;
      while(curr)
      {
         curr->element->OnUpdate(deltaTime);
         curr->element->Draw();
         curr = curr->next;
      }
   }

   void UIManager::OnEvent(Event& e)
   {
      UIElementNode* curr = m_Elements;
      while(curr)
      {
         curr->element->OnEvent(e);
         curr = curr->next;
      }
   }

   void UIManager::Forget(UIElement* element)
   {
      for(UIElementNode** curr = &m_Elements;
            *curr;
            curr = &((*curr)->next))
      {
         if((*curr)->element == element)
         {
            UIElementNode* savedNode = *curr;
            *curr = (*curr)->next;
            savedNode->element = nullptr;
            delete savedNode;
            break;
         }
      }
   }

   Renderer* UIManager::GetRenderer() const
   {
      return m_Renderer;
   }


   Button* UIManager::CreateButton(ButtonType type, IClickListener* listener)
   {
      Button* button = nullptr;

      switch(type)
      {
         case ButtonType::BoxButton:
            button = new BoxButton(this, listener);
            break;
         default:
            LOG_ERROR("Used unimplemented button! Type: %d", type);
      }

      AddElement(button);
      return button;
   }
}
