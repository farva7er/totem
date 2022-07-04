#ifndef _TOTEM_UI_ELEMENT_H_
#define _TOTEM_UI_ELEMENT_H_

#include "Events.h"
#include "Rendering/Renderer.h"


namespace totem
{
   class Element
   {
      public:
         Element(Element* parent = nullptr) : m_Parent(parent) {}

         Element(const Element& other) = default;
         Element& operator=(const Element& other) = default;
         virtual ~Element() = default;

         Element* GetParent() const { return m_Parent; }
         void SetParent(Element* parent) { m_Parent = parent; }

         const math::vec2f& GetPos() const { return m_Pos; }
         void SetPos(const math::vec2f& pos) { m_Pos = pos; }

         const math::vec2f& GetScale() const { return m_Scale; }
         void SetScale(const math::vec2f& scale) { m_Scale = scale; }

         virtual void OnUpdate(float deltaTime) = 0;
         virtual void OnEvent(Event& e) = 0;
         virtual void Draw(Renderer* renderer) const = 0;

      private:
         Element* m_Parent;
         math::vec2f m_Pos;
         math::vec2f m_Scale;
   };
}

#endif
