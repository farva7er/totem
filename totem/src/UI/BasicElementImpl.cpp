#include "BasicElementImpl.h"

namespace totem
{
   BasicElementImpl::~BasicElementImpl()
   {
      if(m_Parent)
      {
         m_Parent->Forget(m_ID);
      }
   }

   BasicElementImpl::BasicElementImpl(const BasicElementImpl& other)
   {
      m_ID = IUIElement::ObtainID();
      m_Parent = nullptr;
      m_Pos = other.m_Pos;
      m_Scale = other.m_Scale;
   }

   BasicElementImpl&
   BasicElementImpl::operator=(const BasicElementImpl& other)
   {
      m_ID = IUIElement::ObtainID();
      m_Parent = nullptr;
      m_Pos = other.m_Pos;
      m_Scale = other.m_Scale;
      return *this;
   }

   void BasicElementImpl::SetPos(const math::vec2f& pos)
   {
      m_Pos = pos;
   }

   void BasicElementImpl::SetScale(const math::vec2f& scale)
   {
      m_Scale = scale;
   }
}
