#ifndef _TOTEM_IIELISTENER_H_
#define _TOTEM_IIELISTENER_H_

namespace totem
{
   class IIEListener
   {
   public:
      IIEListener() = default;
      virtual ~IIEListener() = default;
      IIEListener(const IIEListener& other) = default;
      IIEListener& operator=(const IIEListener& other) = default;

      virtual IIEListener* Clone() const = 0;

      virtual void OnClick(int /*btn*/) {}
      virtual void OnLostHover() {}
      virtual void OnHover() {}
      virtual void OnPush() {}
   };
}
#endif
