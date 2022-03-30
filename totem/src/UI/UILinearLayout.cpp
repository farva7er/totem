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
         m_Scale({1, 0}),
         m_Spacing(0)
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
      m_Pos = pos;
      m_FreeSlotPos += delta.y;
   }

   void UILinearLayout::SetScale(const math::vec2f& scale)
   {
      // TODO Use scale.y
      m_Scale.x = scale.x;
      UIElementNode* curr = m_Elements;
      while(curr)
      {
         totem::math::vec2f elScale(scale.x, curr->element->GetScale().y);
         curr->element->SetScale(elScale);
         curr = curr->next;
      }
   }

   void UILinearLayout::SetSpacing(float spacing)
   {
      m_Spacing = spacing;
   }

   void UILinearLayout::AddElement(IUIElement* element)
   {
      math::vec2f elPos(m_Pos.x, m_FreeSlotPos - element->GetScale().y);
      element->SetScale({ m_Scale.x, element->GetScale().y });
      element->SetPos(elPos);
      m_FreeSlotPos -= 2 * element->GetScale().y + m_Spacing;
      m_Pos.y -= element->GetScale() .y + m_Spacing/2;
      m_Scale.y += element->GetScale().y + m_Spacing/2;
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

   void UILinearLayout::Draw(Renderer* renderer) const
   {
      UIElementNode* curr = m_Elements;
      while(curr)
      {
         curr->element->Draw(renderer);
         curr = curr->next;
      }
   }


   void UILinearLayout::OnEvent(Event& e)
   {
      UIElementNode* curr = m_Elements;
      while(curr)
      {
         curr->element->OnEvent(e);
         curr = curr->next;
      }
   }
}
