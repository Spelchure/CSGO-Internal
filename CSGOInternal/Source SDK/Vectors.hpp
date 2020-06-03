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
    
    const struct _Vector& operator+(struct _Vector another)
    {
        this->x += another.x;
        this->y += another.y;
        this->z += another.z;
        return *this;
    }

    float magnitude() {
        return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
    }

}Vector,Vector3;

float vecGet3DDistance(Vector source, Vector dest);
Vector vecCalcAngles(Vector source, Vector dest);



