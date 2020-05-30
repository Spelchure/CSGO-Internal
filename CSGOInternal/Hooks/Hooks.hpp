/*****************************************************************//**
 * \file   Hooks.hpp
 * \brief  
 * 
 * \author ALPEREN
 * \date   May 2020
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
#include "Memory.hpp"

/**
 * \brief Middle function hook instance
 * 
 * <DetailedDescriptionGoeshere>
 */
class MidFunctionHook
{
    byte_t* originalBytes;
    size_t rsize;
public:
    uintptr_t hookJumpBack;

    MidFunctionHook() : hookJumpBack(0), originalBytes(nullptr), rsize(0) {}
    ~MidFunctionHook() {
        // Dehook
        if (originalBytes != nullptr)
        {
            DWORD dwOldProtect;
            VirtualProtect((byte_t*)(hookJumpBack - 5), rsize, PAGE_EXECUTE_READWRITE, &dwOldProtect);
            memcpy_s((byte_t*)(hookJumpBack - 5), rsize, originalBytes, rsize); // Dehook  remove jump
            VirtualProtect((byte_t*)(hookJumpBack - 5), rsize, dwOldProtect, &dwOldProtect);
        }
    }
    void BeginHook(uintptr_t addy, uintptr_t func, const size_t size);

};


bool hooksDetour(byte_t* src, byte_t* dst, const size_t size); // Classic detour hook
byte_t* hooksTrampoline(byte_t* src, byte_t* dst, const size_t size); // Trampoline hook (using gateway function)
