#ifndef _TOTEM_BASIC_ELEMENT_IMPL_H_
#define _TOTEM_BASIC_ELEMENT_IMPL_H_

#include "IUIElement.h"
#include "Math/Vec.h"

namespace totem
{
   class BasicElementImpl
   {
   public:
      BasicElementImpl()
         :  m_ID(IUIElement::ObtainID()),
            m_Parent(nullptr),
            m_Pos(math::vec2f(0, 0)),
            m_Scale(math::vec2f(1, 1))
      {}

      virtual ~BasicElementImpl();
      BasicElementImpl(const BasicElementImpl& other);
      BasicElementImpl& operator=(const BasicElementImpl& other);

      virtual unsigned int GetID() const
      { return m_ID; }
      
      virtual const math::vec2f& GetPos() const { return m_Pos; }
      virtual const math::vec2f& GetScale() const { return m_Scale; }

      virtual void SetPos(const math::vec2f& pos);
      virtual void SetScale(const math::vec2f& scale);

      virtual IUIElement* GetParent() const { return m_Parent; }
      virtual void SetParent(IUIElement* el) { m_Parent = el; }

      // By default BasicElementImpl is not a parent
      // so Forget just does nothing
      virtual void Forget(unsigned int /*elID*/) {}
 
   protected: 
      unsigned int m_ID;
      IUIElement* m_Parent;
      math::vec2f m_Pos;
      math::vec2f m_Scale;
   };
}

#endif
