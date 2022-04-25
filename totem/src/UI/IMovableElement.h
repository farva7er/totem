#ifndef _TOTEM_IMOVABLE_ELEMENT_H_
#define _TOTEM_IMOVABLE_ELEMENT_H_

#include "IUIElement.h"

namespace totem
{
   class IMovableElement : public IUIElement
   {
   public:
      virtual ~IMovableElement() = default;

      virtual const math::vec2f& GetPos() const = 0;
      virtual const math::vec2f& GetScale() const = 0;

      virtual void SetPos(const math::vec2f& pos) = 0;
      virtual void SetScale(const math::vec2f& scale) = 0;

   };
}

#endif
