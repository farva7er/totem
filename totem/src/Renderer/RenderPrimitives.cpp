#include "RenderPrimitives.h"

namespace totem
{

   Rect::Builder::Builder()
   {
      m_Result = new Rect();
   }

   Rect::Builder::~Builder()
   {
      delete m_Result;
   }

   Rect::Builder& Rect::Builder::SetPos(const math::vec2f& pos)
   {
      m_Result->m_Pos = pos;
      return *this;
   }

   Rect::Builder& Rect::Builder::SetScale(const math::vec2f& scale)
   {
      m_Result->m_Scale = scale;
      return *this;
   }

   Rect::Builder& Rect::Builder::SetColor(const math::vec4f& color)
   {
      m_Result->m_Color = color;
      return *this;
   }

   Rect::Builder& Rect::Builder::SetImagePath(const char* imagePath)
   {
      if(m_Result->m_Texture)
      {
         LOG_ERROR("Cannot set texture and image at once: %s", imagePath);
         return *this;
      }

      m_Result->m_ImagePath = new char[strlen(imagePath) + 1];
      strcpy(m_Result->m_ImagePath, imagePath);
      return *this;
   }

   Rect::Builder& Rect::Builder::SetTexture(const Texture* texture)
   {
      if(m_Result->m_ImagePath)
      {
         LOG_ERROR("Cannot set image and texture at once: %s",
                                       m_Result->m_ImagePath);
         return *this;
      }
      m_Result->m_Texture = texture;
      return *this;
   }

   Rect::Builder& Rect::Builder::SetShaderId(const char* shaderId)
   {
      m_Result->m_ShaderId = new char[strlen(shaderId) + 1];
      strcpy(m_Result->m_ShaderId, shaderId);
      return *this;
   }

   Rect::Builder& Rect::Builder::SetRotationAngle(float degAngle)
   {
      m_Result->m_RotationAngleDeg = degAngle;
      return *this;
   }

   Rect::Builder& Rect::Builder::SetRotationAxis(const math::vec2f& axis)
   {
      m_Result->m_RotationAxis = axis;
      return *this;
   }

   Rect Rect::Builder::Construct()
   {
      return *m_Result;
   }

   void Rect::Builder::Reset()
   {
      if(m_Result)
         delete m_Result;
      m_Result = new Rect();
   }

   math::vec2f Rect::GetPos() const
   {
      return m_Pos;
   }

   math::vec2f Rect::GetScale() const
   {
      return m_Scale;
   }

   math::vec4f Rect::GetColor() const
   {
      return m_Color;
   }

   const char* Rect::GetImagePath() const
   {
      return m_ImagePath;
   }

   const Texture* Rect::GetTexture() const
   {
      return m_Texture;
   }

   const char* Rect::GetShaderId() const
   {
      return m_ShaderId;
   }

   float Rect::GetRotationAngle() const
   {
      return m_RotationAngleDeg;
   }

   math::vec2f Rect::GetRotationAxis() const
   {
      return m_RotationAxis;
   }

   Rect::Rect()
      :  m_Pos(0, 0), m_Scale(1, 1), m_Color(1, 1, 1, 1),
         m_ImagePath(nullptr), m_Texture(nullptr), m_ShaderId(nullptr),
         m_RotationAngleDeg(0), m_RotationAxis(0, 0)
   {}

   Rect::Rect(const Rect& other)
   {
      m_ImagePath = nullptr;
      if(other.m_ImagePath)
      {
         m_ImagePath = new char[strlen(other.m_ImagePath) + 1];
         strcpy(m_ImagePath, other.m_ImagePath);
      }
      m_ShaderId = nullptr;
      if(other.m_ShaderId)
      {
         m_ShaderId = new char[strlen(other.m_ShaderId) + 1];
         strcpy(m_ShaderId, other.m_ShaderId);
      }

      m_Texture = other.m_Texture;
      m_Pos = other.m_Pos;
      m_Scale = other.m_Scale;
      m_Color = other.m_Color;
      m_RotationAngleDeg = other.m_RotationAngleDeg;
      m_RotationAxis = other.m_RotationAxis;
   }

   Rect& Rect::operator=(const Rect& other)
   {
      if(&other == this)
         return *this;

      if(m_ImagePath)
         delete [] m_ImagePath;
      m_ImagePath = nullptr;
      if(other.m_ImagePath)
      {
         m_ImagePath = new char[strlen(other.m_ImagePath) + 1];
         strcpy(m_ImagePath, other.m_ImagePath);
      }

      if(m_ShaderId)
         delete [] m_ShaderId;
      m_ShaderId = nullptr;
      if(other.m_ShaderId)
      {
         m_ShaderId = new char[strlen(other.m_ShaderId) + 1];
         strcpy(m_ShaderId, other.m_ShaderId);
      }
      
      m_Texture = other.m_Texture;
      m_Pos = other.m_Pos;
      m_Scale = other.m_Scale;
      m_Color = other.m_Color;
      m_RotationAngleDeg = other.m_RotationAngleDeg;
      m_RotationAxis = other.m_RotationAxis;
      return *this;
   }

   Rect::~Rect()
   {
      if(m_ImagePath)
         delete [] m_ImagePath;
      if(m_ShaderId)
         delete [] m_ShaderId;
   }
}


