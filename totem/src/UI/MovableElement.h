#ifndef _TOTEM_BASE_ELEMENT_H_
#define _TOTEM_BASE_ELEMENT_H_

#include "IUIElement.h"

namespace totem
{

   class IMovableElement : public IUIElement
   {
   public:
      virtual ~IMovableElement() {}

      virtual const math::vec2f& GetPos() const = 0;
      virtual const math::vec2f& GetScale() const = 0;

      virtual void SetPos(const math::vec2f& pos) = 0;
      virtual void SetScale(const math::vec2f& scale) = 0;

   };

   class MovableElement : public IMovableElement
   {
   public:
      MovableElement()
         :  m_Pos(math::vec2f(0, 0)),
            m_Scale(math::vec2f(1, 1))
      {}

      virtual ~MovableElement() = default;
      MovableElement(const MovableElement& other) = default;
      MovableElement& operator=(const MovableElement& other) = default;
      
      virtual const math::vec2f& GetPos() const override { return m_Pos; }
      virtual const math::vec2f& GetScale() const override { return m_Scale; }

      virtual void SetPos(const math::vec2f& pos) override;
      virtual void SetScale(const math::vec2f& scale) override;

   protected: 
      math::vec2f m_Pos;
      math::vec2f m_Scale;
   };
}

#endif
