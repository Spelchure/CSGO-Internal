/*****************************************************************//**
 * \file   Vectors.hpp
 * \brief  
 * 
 * \author ALPEREN
 * \date   June 2020
 * 
 *  <br>
 *  This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *********************************************************************/
#pragma once
#include <cmath>

#define DECL_ALIGN(x) __declspec(align(x))
#define ALIGN16 DECL_ALIGN(16)
#define ALIGN16_POST DECL_ALIGN(16)

typedef struct _Vector {
    float x, y, z;
    struct _Vector(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    const struct _Vector & operator-(struct _Vector another)
    {
        this->x -= another.x;
        this->y -= another.y;
        this->z -= another.z;
        return *this;
    }
   /* struct _Vector & operator =(struct _Vector &v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }*/
    struct _Vector operator-(const struct _Vector &v) const {
        return struct _Vector(x - v.x, y - v.y, z - v.z);
    }
    const struct _Vector& operator+(struct _Vector another)
    {
        this->x += another.x;
        this->y += another.y;
        this->z += another.z;
        return *this;
    } 
    struct _Vector operator+(const struct _Vector &v) const {
        return struct _Vector(x + v.x, y + v.y, z + v.z);
    }
    struct _Vector & operator -=(const struct _Vector & v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }
    struct _Vector & operator *=(const struct _Vector &v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }
    struct _Vector operator /(float f) {
        float nx, ny, nz;
        nx = x / f; ny = y / f; nz = z / f;
        return struct _Vector(nx, ny, nz);
    }

    struct _Vector Unit()
    {
        float nx, ny, nz;
        float ln = this->magnitude();
    
        nx = this->x / ln;
        ny = this->y / ln;
        nz = this->z / ln;
      
        return struct _Vector(nx, ny, nz);
    }
    void Clamp()
    {
        if (x > 89.0f && x <= 180.0f)
            x = 89.0f;
        if (x > 180.0f)
            x = x - 360.0f;
        if (x < -89.0f)
            x = -89.0f;
        if (y > 180.0f)
            y = y - 360.0f;
        if (y < -180.0f)
            y = y + 360.0f;
   
    }
    void Normalize()
    {
        while(this->x > 180.0f) 
            this->x -= 360.0f;
        while (this->x < -180.0f)
            this->x += 360.0f;
        while(this->y > 180.0f) 
            this->y -= 360.0f;
        while (this->y < -180.0f)
            this->y += 360.0f;
        
        this->z = 0;
    }
        
    float magnitude() {
        return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
    }

    void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
    {
        x = ix; y = iy; z = iz;
    }

    float LengthSqr(void) const
    {
        return (x * x + y * y + z * z);
    }

    float& operator[](int i)
    {
        return ((float*)this)[i];
    }
    float operator[](int i) const
    {
        return ((float*)this)[i];
    }


}Vector,Vector3;

typedef struct _Vector2
{
    float x, y;

    struct _Vector2(float x = 0, float y = 0) : x(x),y(y) {}

  

}Vector2;

class ALIGN16 VectorAligned : public Vector
{
public:
    inline VectorAligned(void) {};

    inline VectorAligned(Vector X, Vector Y, Vector Z)
    {
        //Init(X, Y, Z);
    }

    explicit VectorAligned(const Vector& vOther)
    {
        Init(vOther.x, vOther.y, vOther.z);
    }

    VectorAligned& operator=(const Vector& vOther)
    {
        Init(vOther.x, vOther.y, vOther.z);
        return *this;
    }

    float w;	// this space is used anyway
} ALIGN16_POST;


float vecGet3DDistance(Vector source, Vector dest);
Vector vecCalcAngles(Vector source, Vector dest);



