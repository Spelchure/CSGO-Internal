/*****************************************************************//**
 * \file   Hooks.cc
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
#include <Windows.h>
#include "Hooks.hpp"

/**
 * \brief Classic detour hook aka jmp hook 
 *
 * <Detailed description goes here> 
 * 
 * \param src Source address to hook
 * \param dst Destination address to jmp
 * \param size size in bytes
 * \returns True on success 
 * \returns False on invalid paramater
 */
bool 
hooksDetour(byte_t* src, byte_t* dst, const size_t size)
{
    if (size < 5) return false; // Size must be greater then 5 because jmp <addy> statement 5 bytes.
    DWORD dwOldProtect;

    VirtualProtect(src, size, PAGE_EXECUTE_READWRITE, &dwOldProtect);
    uintptr_t relAddy = dst - src - 5; // destination - source - SIZE_OF_JUMP

    *(byte_t*)(src) = (byte_t)0xE9;   // JMP Statement
    *(uintptr_t*)(src + 1) = relAddy; // JMP What ?

    VirtualProtect(src, size, dwOldProtect, &dwOldProtect);
    return true;
}

/**
 * \brief Trampoline(Gateway) hook 
 * 
 * <DetailedDescriptionGoesHere>
 * 
 * \param src Source address to hook  
 * \param dst Destination address to jmp 
 * \param size size in bytes
 * \return Address of gateway function , nullptr if failure
 */
byte_t
*hooksTrampoline(byte_t *src, byte_t *dst,  const size_t size)
{
    if (size < 5) return nullptr;

    // Allocate gateway function, select address automatically
    byte_t* gateway = (byte_t*)VirtualAlloc(nullptr, size + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

    // Copy stolen bytes from src to gateway function
    memcpy_s(gateway, size, src, size);

    uintptr_t gatewayRelativeAddy = (uintptr_t)(src - gateway - 5);
    *(gateway + size) = (byte_t)0xE9; // JMP statement
    *(uintptr_t*)((uintptr_t)gateway + size + 1) = gatewayRelativeAddy;

    if (hooksDetour(src, dst, size)) // Classical detour hook
        return gateway;
    return nullptr;
}