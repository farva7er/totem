#include <math.h>
#include "Mat.h"

#ifndef M_PI
   #define M_PI 3.14159265358979323846
#endif

namespace totem
{
   namespace math
   {
      
      mat4f operator* (const mat4f& a, const mat4f& b)
      {
         mat4f res(0);
         for(int i = 0; i < 4; i++)
         {
            for(int j = 0; j < 4; j++)
            {
               for(int k = 0; k < 4; k++)
               {
                  res[i][j] += a[i][k] * b[k][j];
               }
            }
         }
         return res;
      }

      mat4f getOrthoProj(float l, float r, float t, float b, float n, float f)
      {
         mat4f mat;
         mat[0][0] = 2.0f/(r-l);
         mat[0][3] = -(r+l)/(r-l);
         mat[1][1] = 2.0f/(t-b);
         mat[1][3] = -(t+b)/(t-b);
         mat[2][2] = -2/(f-n);
         mat[2][3] = -(f+n)/(f-n);
         return mat;
      }

      mat4f getOrthoProj(float r, float t, float n, float f)
      {
         return getOrthoProj(-r, r, t, -t, n, f);
      }

      mat4f getRotationZ(float radians)
      {
         mat4f mat;
         mat[0][0] = cos(radians);
         mat[0][1] = -sin(radians);
         mat[1][0] = sin(radians);
         mat[1][1] = cos(radians);
         mat[2][2] = 1;
         mat[3][3] = 1;
         return mat;
      }

      mat4f getTranslate(float tx, float ty, float tz)
      {
         mat4f mat;
         mat[0][3] = tx;
         mat[1][3] = ty;
         mat[2][3] = tz;
         return mat;
      }

      mat4f getTranslate(float tx, float ty)
      {
         return getTranslate(tx, ty, 0.0f);
      }

      mat4f getScale(float sx, float sy)
      {
         mat4f mat;
         mat[0][0] = sx;
         mat[1][1] = sy;
         return mat;
      }

      mat4f getScale(float s)
      {  
         return getScale(s, s);
      }

      float degToRad(float angle)
      {
         return (angle * M_PI) / 180;
      }
   }
}
