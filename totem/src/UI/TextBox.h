#ifndef _TOTEM_TEXTBOX_H_
#define _TOTEM_TEXTBOX_H_

#include "IMovableElement.h"
#include "BasicElementImpl.h"
#include "Internationalization/Text.h"

namespace totem
{
   class TextBox : public IMovableElement, public BasicElementImpl
   {
   public:
      TextBox(Ref<Font> font);
      TextBox(const TextBox& other) = default;
      ~TextBox() = default;

      //TODO
      //Text GetText() const { return m_TextWords.AssembleText(); }
      virtual void SetText(const Text& text);

      void SetLineSpacing(float spacing);
      void SetFont(const Font* font);
      void SetFontSize(float size);
      void SetTextColor(const math::vec4f& color);
      void SetCharDisplayLimit(int charLimit);


      virtual unsigned int GetID() const
      { return BasicElementImpl::GetID(); }

      virtual IUIElement* GetParent() const
      { return BasicElementImpl::GetParent(); }

      virtual void SetParent(IUIElement* el)
      { BasicElementImpl::SetParent(el); }

      // A way to get notified by a child that it is going to be deleted
      virtual void Forget(unsigned int elID)
      { BasicElementImpl::Forget(elID); }

      virtual void Draw(Renderer* renderer) const;

      virtual const math::vec2f& GetPos() const
      { return BasicElementImpl::GetPos(); }
      virtual const math::vec2f& GetScale() const
      { return BasicElementImpl::GetScale(); }

      virtual void SetPos(const math::vec2f& pos)
      { BasicElementImpl::SetPos(pos); }
      virtual void SetScale(const math::vec2f& scale)
      { BasicElementImpl::SetScale(scale); }

      virtual void OnEvent(Event& /*e*/) override {}

   private:
      Text::TextVec m_TextWords;
      float m_LineSpacing;
      Ref<Font> m_Font;
      float m_FontSize;
      math::vec4f m_TextColor;
      int m_CharDisplayLimit;
   };
}

#endif
