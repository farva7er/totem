#include "Clickable.h"

namespace totem
{
   Clickable::Clickable(Element* parent)
      : Element(parent), m_State(State::Default), m_IsActive(true)
   {}

   Clickable::Clickable(const Clickable& other)
      : Element(other)
   {
      m_State = State::Default;
      m_IsActive = other.m_IsActive;
   }

   Clickable& Clickable::operator=(const Clickable& other)
   {
      Element::operator=(other);
      m_State = State::Default;
      m_IsActive = other.m_IsActive;
      return *this;
   }

   bool Clickable::IsActive() const
   {
      return m_IsActive;
   }

   void Clickable::SetActive(bool isActive)
   {
      m_IsActive = isActive;
      if(!isActive)
      {
         m_State = State::Default;
         OnDefault();
      }
   }

   void Clickable::OnEvent(Event& e)
   {
      if(!m_IsActive)
         return;

      EventDispatcher<Clickable> d(this);

      d.Dispatch<MouseMoveEvent>(&Clickable::OnMouseMove, e);
      d.Dispatch<MousePressedEvent>(&Clickable::OnMousePressed, e);
      d.Dispatch<MouseReleasedEvent>(&Clickable::OnMouseReleased, e);
   }

   void Clickable::OnMouseMove(MouseMoveEvent& e)
   {
      math::vec2f mouseCanvasCoords(e.GetX(), e.GetY());
      bool isHovered = IsHovered(mouseCanvasCoords);
      if(m_State == State::Default && isHovered)
      {
         m_State = State::Hovered;
         OnHover();
      }

      if((m_State == State::Hovered || m_State == State::Pushed) &&
         !isHovered)
      {
         m_State = State::Default;
         OnDefault();
      }
   }

   void Clickable::OnMousePressed(MousePressedEvent& e)
   {
      if(m_State == State::Hovered && e.GetButton() == 0) // Left Mouse Button
      {
         m_State = State::Pushed;
         OnPush();
         e.SetHandled();
      }
   }

   void Clickable::OnMouseReleased(MouseReleasedEvent& e)
   {
      if(m_State == State::Pushed && e.GetButton() == 0) // Left Mouse Button
      {
         OnClick();
         if(m_IsActive)
         {
            m_State = State::Hovered;
            OnHover();
         }
         e.SetHandled();
      }
   }

   bool Clickable::IsHovered(const math::vec2f& mouseCanvasCoords) const
   {
      const math::vec2f pos = GetPos();
      const math::vec2f scale = GetScale();
      if(mouseCanvasCoords.x >= pos.x - scale.x &&
         mouseCanvasCoords.x <= pos.x + scale.x &&
         mouseCanvasCoords.y >= pos.y - scale.y &&
         mouseCanvasCoords.y <= pos.y + scale.y)
      {
         return true;
      }
      return false;
   }
}
