#ifndef _TOTEM_MAT_H_
#define _TOTEM_MAT_H_

namespace totem
{
   namespace math
   {
      // 4x4 matrix of T's, row major order
      // Ex.: mat4f m; m[0][1] - first row, second column
      // (indexed from 0)

      template <typename T>
      struct mat4
      {
         // identity by default
         mat4()
            : mat4(1)
         {}

         // scalar matrix
         mat4(T val)
            : values {
                        {val, 0, 0, 0},
                        {0, val, 0, 0},
                        {0, 0, val, 0},
                        {0, 0, 0, val}
                     }
         {}


         // get i-th row
         T* operator[] (unsigned int i) { return values[i]; }
         const T* operator[] (unsigned int i) const { return values[i]; }

         // get one-dimensional array of 16 floats
         T* ToArray() { return values[0]; }
         const T* ToArray() const { return values[0]; }

      public:
         T values[4][4];
      };

      typedef mat4<float> mat4f;
      
      mat4f operator* (const mat4f& a, const mat4f& b);

      mat4f getOrthoProj(float l, float r, float t, float b, float n, float f);
      mat4f getOrthoProj(float r, float t, float n, float f);
      mat4f getRotationZ(float radians);
      mat4f getTranslate(float tx, float ty, float tz);
      mat4f getTranslate(float tx, float ty);
      mat4f getScale(float sx, float sy);
      mat4f getScale(float s);

      float degToRad(float angle);
   }
}
#endif
