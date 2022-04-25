#include "UILinearLayout.h"

namespace totem
{
   UILinearLayout::ElementNode::~ElementNode()
   {
      delete element;
   }

   UILinearLayout::UILinearLayout()
      :  m_Elements(nullptr),
         m_Spacing(0)
   {
      m_FreeSlotPos = m_Pos.y;
   }

   UILinearLayout::~UILinearLayout()
   {
      while(m_Elements)
      {
         ElementNode* savedNode = m_Elements;
         m_Elements = m_Elements->next;
         delete savedNode;
      }
   }

   void UILinearLayout::SetPos(const math::vec2f& pos)
   {
      math::vec2f delta = pos - m_Pos;
      ElementNode* curr = m_Elements;
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
      ElementNode* curr = m_Elements;
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

   void UILinearLayout::AddElement(IMovableElement* element)
   {
      element->SetParent(this);

      math::vec2f elPos(m_Pos.x, m_FreeSlotPos - element->GetScale().y);
      element->SetScale({ m_Scale.x, element->GetScale().y });
      element->SetPos(elPos);
      m_FreeSlotPos -= 2 * element->GetScale().y + m_Spacing;
      m_Pos.y -= element->GetScale() .y + m_Spacing/2;
      m_Scale.y += element->GetScale().y + m_Spacing/2;
      m_Elements = new ElementNode(element, m_Elements);
   }

   void UILinearLayout::Forget(unsigned int elID)
   {
      ElementNode** curr = &m_Elements;
      while(*curr)
      {
         //LOG_INFO("FORGET: %u , %u", elID, (*curr)->element->GetID());
         if(elID  == (*curr)->element->GetID())
         {
            (*curr)->element = nullptr; // Don't need to delete it
            ElementNode* savedNode = *curr;
            *curr = (*curr)->next;
            delete savedNode;
            return;
         }
         curr = &((*curr)->next);
      }
   }

   void UILinearLayout::OnUpdate(float deltaTime)
   {
      ElementNode* curr = m_Elements;
      while(curr)
      {
         curr->element->OnUpdate(deltaTime);
         curr = curr->next;
      }
   }

   void UILinearLayout::Draw(Renderer* renderer) const
   {
      ElementNode* curr = m_Elements;
      while(curr)
      {
         curr->element->Draw(renderer);
         curr = curr->next;
      }
   }


   void UILinearLayout::OnEvent(Event& e)
   {
      ElementNode* curr = m_Elements;
      while(curr)
      {
         curr->element->OnEvent(e);
         curr = curr->next;
      }
   }
}
