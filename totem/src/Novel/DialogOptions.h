#ifndef _TOTEM_NOVEL_DIALOG_OPTIONS_H_
#define _TOTEM_NOVEL_DIALOG_OPTIONS_H_

#include "Internationalization/Text.h"

namespace totem
{
   class DialogOptions
   {
      public:
         DialogOptions();
         ~DialogOptions();
         
         void Add(const Text& text);

         const Text* GetOptionText() const;
         void Next();

      private:
         struct OptionNode
         {
            const Text m_OptionText;
            OptionNode* m_Next;
            OptionNode(const Text& text, OptionNode* next);
         };
         OptionNode* m_OptionsHead;
         OptionNode* m_OptionsTail;
         OptionNode* m_CurrentNode;
   };
}

#endif
