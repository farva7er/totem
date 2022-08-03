#include "DialogOptions.h"

namespace totem
{
   DialogOptions::OptionNode::OptionNode(const Text& text, OptionNode* next)
      : m_OptionText(text), m_Next(next)
   {}

   DialogOptions::DialogOptions()
      : m_OptionsHead(nullptr), m_OptionsTail(nullptr),
      m_CurrentNode(nullptr)
   {}

   DialogOptions::~DialogOptions()
   {
      OptionNode* tmp;
      while(m_OptionsHead)
      {
         tmp = m_OptionsHead;
         m_OptionsHead = m_OptionsHead->m_Next;
         delete tmp;
      }
   }

   void DialogOptions::Add(const Text& text)
   {
      if(!m_OptionsHead)
      {
         m_OptionsHead = new OptionNode(text, nullptr);
         m_OptionsTail = m_OptionsHead;
         m_CurrentNode = m_OptionsHead;
      }
      else
      {
         m_OptionsTail->m_Next = new OptionNode(text, nullptr);
         m_OptionsTail = m_OptionsTail->m_Next;
      }
   }

   const Text* DialogOptions::GetOptionText() const
   {
      if(!m_CurrentNode)
         return nullptr;
      return &(m_CurrentNode->m_OptionText);
   }

   void DialogOptions::Next()
   {
      if(m_CurrentNode)
         m_CurrentNode = m_CurrentNode->m_Next;
   }
}
