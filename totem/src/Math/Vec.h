#ifndef _TOTEM_MATH_H_
#define _TOTEM_MATH_H_

#include <string.h>

#include "Assert.h"

namespace totem
{
   namespace math
   {
      template <size_t dim, typename T>
      struct vec {
         public:
            vec()
            {
               memset(coord, 0, sizeof(coord));
            }
            T& operator[] (size_t i)
            {
               TOTEM_ASSERT(i < dim, "vec out of bounds");
               return coord[dim];
            }
            const T& operator[] (size_t i) const
            {
               TOTEM_ASSERT(i < dim, "vec out of bounds");
               return coord[dim];
            }

         private:

            T coord[dim];
      };

      template <typename T>
      struct vec<2, T> {
         vec() : x(0), y(0) {}
         vec(T x, T y) : x(x), y(y) {}
         T& operator[] (size_t i)
         {
            TOTEM_ASSERT(i < 2, "vec out of bounds");
            return i == 0 ? x : y;
         }
         const T& operator[] (size_t i) const
         {
            TOTEM_ASSERT(i < 2, "vec out of bounds");
            return i == 0 ? x : y;
         }

         T x, y;
      };

      template <typename T>
      struct vec<3, T> {
         vec() : x(0), y(0), z(0) {}
         vec(T x, T y, T z) : x(x), y(y), z(z) {}
         T& operator[] (size_t i)
         {
            TOTEM_ASSERT(i < 3, "vec out of bounds");
            return i == 0 ? x : ( i == 1 ? y : z);
         }
         const T& operator[] (size_t i) const
         {
            TOTEM_ASSERT(i < 3, "vec out of bounds");
            return i == 0 ? x : ( i == 1 ? y : z);
         }

         T mod() const
         {
            return sqrt(x * x + y * y + z * z);
         }

         vec<3,T> normalize() const
         {
            T _mod = mod();
            return vec<3, T>(x / _mod, y / _mod, z / _mod);
         }

         T x, y, z;
      };

      template <typename T>
      struct vec<4, T> {
         vec() : x(0), y(0), z(0), w(0) {}
         vec(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
         T& operator[] (size_t i)
         {
            TOTEM_ASSERT(i < 4, "vec out of bounds");
            return i == 0 ? x : ( i == 1 ? y : ( i == 2 ? z : w ));
         }
         const T& operator[] (size_t i) const
         {
            TOTEM_ASSERT(i < 4, "vec out of bounds");
            return i == 0 ? x : ( i == 1 ? y : ( i == 2 ? z : w ));
         }

         T x, y, z, w;
      };

      typedef vec<2, float> vec2f;
      typedef vec<3, float> vec3f;
      typedef vec<4, float> vec4f;

      typedef vec<2, int> vec2i;
      typedef vec<3, int> vec3i;
      typedef vec<4, int> vec4i;




      //sum of two vectors a+b
      template <size_t dim, typename T>
      vec<dim, T> operator+ (const vec<dim, T>& a, const vec<dim, T>& b)
      {
         vec<dim, T> res = a;
         for(size_t i = 0; i < dim; i++)
            res[i] += b[i];
         return res;
      }


      //difference of two vectors a-b
      template <size_t dim, typename T>
      vec<dim, T> operator- (const vec<dim, T>& a, const vec<dim, T>& b)
      {
         vec<dim, T> res = a;
         for(size_t i = 0; i < dim; i++)
            res[i] -= b[i];
         return res;
      }

      //dot product of two vectors a*b
      template <size_t dim, typename T>
      T operator* (const vec<dim, T> &a, const vec<dim, T> &b)
      {
         T res(0);
         for(size_t i = 0; i < dim; i++)
            res += a[i] * b[i];
         return res;
      }

      //unary minus -a
      template <size_t dim, typename T>
      vec<dim, T> operator- (const vec<dim, T>& v)
      {
         vec<dim, T> res = v;
         for(size_t i = 0; i < dim; i++)
            res[i] = -(res[i]);
         return res;
      }

      //multiply vector by a scalar value
      template <size_t dim, typename T, typename U>
      vec<dim, T> operator* (const vec<dim, T>& v, U scalar)
      {
         vec<dim, T> res = v;
         for(size_t i = 0; i < dim; i++)
            res[i] *= scalar;
         return res;
      }

      //multiply vector by a scalar value
      template <size_t dim, typename T, typename U>
      vec<dim, T> operator* (U scalar, const vec<dim, T>& v)
      {
         vec<dim, T> res = v;
         for(size_t i = 0; i < dim; i++)
            res[i] *= scalar;
         return res;
      }
   }
}
#endif
