/*****************************************************************//**
 * \file   Memory.hpp
 * \brief  
 * 
 * \author ALPEREN
 * \date   May 2020
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
#include <cstdint>

typedef int8_t byte_t;

void memPatch(byte_t* dst, byte_t* src, size_t size);
void memFillWithNopes(byte_t* dst, size_t size);
uintptr_t memFindPattern(const char* sModule, const char* pattern, const char* mask);
// uintptr_t find_multilevel_ptr(uintptr_t ptr, std::vector<uint32_t> offsets); Not required function

// byte_t* find_pattern(byte_t* start, const char* pattern, size_t sPattern, const char* match, size_t mSize); Not required also
