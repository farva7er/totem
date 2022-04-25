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

   void BasicElementImpl::SetPos(const math::vec2f& pos)
   {
      m_Pos = pos;
   }

   void BasicElementImpl::SetScale(const math::vec2f& scale)
   {
      m_Scale = scale;
   }
}
