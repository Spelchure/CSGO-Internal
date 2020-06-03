/*****************************************************************//**
 * \file   Vectors.cc
 * \brief  
 * 
 * \author ALPEREN
 * \date   June 2020
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
#include "Vectors.hpp"

#pragma warning(disable : 4244)

// Radian to degrees
#define PI  3.14159265358
#define DEGREES(x) ((x) * (180/PI))


float 
vecGet3DDistance(Vector source, Vector dest)
{
    Vector delta = source - dest;
    return delta.magnitude();
}

Vector 
vecCalcAngles(Vector source, Vector dest)
{
    Vector angle;
   
    Vector delta = dest - source;
    float mag = delta.magnitude();

    angle.x = DEGREES(-asin(delta.z / mag)); // pitch 
    angle.y = DEGREES(atan2(delta.y, delta.x)); // yaw
    angle.z = 0; // roll

    return angle;
}
