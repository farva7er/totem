#include "UIManager.h"
#include "IUIElement.h"

namespace totem
{
   UIManager::UIElementNode::~UIElementNode()
   {
      delete element;
   }

   UIManager::UIManager(Renderer* renderer)
      : m_Elements(nullptr), m_Renderer(renderer)
   {}

   UIManager::~UIManager()
   {
      while(m_Elements)
      {
         UIElementNode* savedNode = m_Elements;
         m_Elements = m_Elements->next;
         delete savedNode;
      }
   }

   void UIManager::AddElement(IUIElement* element)
   {
      m_Elements = new UIElementNode(element, m_Elements);
   }

   void UIManager::OnUpdate(float deltaTime)
   {
      UIElementNode* curr = m_Elements;
      while(curr)
      {
         curr->element->OnUpdate(deltaTime);
         curr->element->Draw(m_Renderer);
         curr = curr->next;
      }
   }

   void UIManager::OnEvent(Event& e)
   {
      if(e.GetType() == EventType::MouseMove)
      {
         MouseMoveEvent& me = e.Cast<MouseMoveEvent>();
         math::vec2f mouseCoords = 
            m_Renderer->ScreenToScene(math::vec2f(me.GetX(), me.GetY()));
         me.SetX(mouseCoords.x);
         me.SetY(mouseCoords.y);
         DispatchEvent(me);
         return;
      }
      else if(e.GetType() == EventType::WindowResize)
      {
         SceneResizeEvent sre(m_Renderer->GetSceneSize().x,
                              m_Renderer->GetSceneSize().y);
         DispatchEvent(sre);
         return;
      }

      DispatchEvent(e);
   }

   void UIManager::DispatchEvent(Event& e) const
   {
      UIElementNode* curr = m_Elements;
      while(curr)
      {

         curr->element->OnEvent(e);
         curr = curr->next;
      }
   }

   Renderer* UIManager::GetRenderer() const
   {
      return m_Renderer;
   }

   IButton* UIManager::CreateButton(ButtonType type)
   {
      IButton* button = nullptr;

      switch(type)
      {
         case ButtonType::SimpleBoxButton:
            button = new BoxButton();
            break;
         case ButtonType::FixedBoxButton:
            button = new BoxButton();
            button = new ButtonColorDecorator(button);
            break;
         case ButtonType::SoftBoxButton:
            button = new BoxButton();
            button = new ButtonColorDecorator(button);
            button = new ButtonScaleDecorator(button);
            break;
         default:
            LOG_ERROR("Used unimplemented button! Type: %d", type);
      }

      AddElement(button);
      return button;
   }
}
