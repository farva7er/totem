#include "UILinearLayout.h"

namespace totem
{
   UILinearLayout::UIElementNode::~UIElementNode()
   {
      delete element;
   }

   UILinearLayout::UILinearLayout()
      :  m_Elements(nullptr),
         m_Pos({0, 0}),
         m_Scale({1, 0})
   {
      m_FreeSlotPos = m_Pos.y;
   }

   UILinearLayout::~UILinearLayout()
   {
      while(m_Elements)
      {
         UIElementNode* savedNode = m_Elements;
         m_Elements = m_Elements->next;
         delete savedNode;
      }
   }

   void UILinearLayout::SetPos(const math::vec2f& pos)
   {
      math::vec2f delta = pos - m_Pos;
      UIElementNode* curr = m_Elements;
      while(curr)
      {
         const math::vec2f& prevPos = curr->element->GetPos();
         curr->element->SetPos(prevPos + delta);
         curr = curr->next;
      }

   }

   void UILinearLayout::SetScale(const math::vec2f& scale)
   {
      // TODO
   }

   void UILinearLayout::AddElement(IUIElement* element)
   {
      math::vec2f elPos(m_Pos.x, m_FreeSlotPos - element.GetScale().y);
      element.SetScale(m_Scale.x, element.GetScale().y);
      element.SetPos(elPos);
      m_FreeSlotPos -= element.GetScale().y;
      m_Scale.y += element.GetScale().y;
      m_Elements = new UIElementNode(element, m_Elements);
   }

   void UILinearLayout::OnUpdate(float deltaTime)
   {
      UIElementNode* curr = m_Elements;
      while(curr)
      {
         curr->element->OnUpdate(deltaTime);
         curr = curr->next;
      }
   }

   void UILinearLayout::Draw(Renderer* renderer)
   {
      UIElementNode* curr = m_Elements;
      while(curr)
      {
         curr->element->Draw(renderer);
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
}
