/*****************************************************************//**
 * \file   Memory.cc
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
#include "Memory.hpp"
#include <Windows.h>
#include <Psapi.h>

/**
 * \brief Patch memory
 * 
 * \param dst Destinition address to patch
 * \param src Source bytes 
 * \param size size in bytes
 */
void
memPatch(byte_t* dst, byte_t* src, size_t size)
{
    DWORD dwOldProtect;

    //Now memory is accessible
    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &dwOldProtect);
    memcpy_s(dst, size, src, size); // Patch!
    VirtualProtect(dst, size, dwOldProtect, &dwOldProtect); // Restore memory
}

/**
 * \brief Fill specified region to nopes(0x90)
 * 
 * \param dst Destinition address
 * \param size size in bytes
 */
void
memFillWithNopes(byte_t *dst, size_t size)
{

    DWORD dwOldProtect;

    VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &dwOldProtect);
    memset(dst, 0x90, size);
    VirtualProtect(dst, size, dwOldProtect, &dwOldProtect);

}

static 
MODULEINFO
memGetModuleInfo(const char* sModule)
{
    MODULEINFO modInfo = { 0 };
    HMODULE hModule = GetModuleHandleA(sModule);

    if (nullptr == hModule)
        return modInfo;

    GetModuleInformation(GetCurrentProcess(), hModule, &modInfo, sizeof(MODULEINFO));
    return modInfo;
}

/**
 * \brief Search pattern in specified module 
 * \param szModule Name of module 
 * \param pattern Pattern to find 
 * \param mask Mask of pattern 
 * 
 * \return Address of pattern if found otherwise 0
 */
uintptr_t 
memFindPattern(const char* sModule, const char* pattern, const char* mask)
{
    MODULEINFO modInf = memGetModuleInfo(sModule);
    if (modInf.SizeOfImage <= 0)
        return 0;

    uintptr_t base = (uintptr_t)modInf.lpBaseOfDll;
    uintptr_t size = (uintptr_t)modInf.SizeOfImage;
    int mask_size = strlen(mask);

    for (uint32_t i = 0; i < size - mask_size; i++)
    {
        bool found = true;
        for(int j = 0; j < mask_size; j++) 
			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
        if (found)
            return base + i;
    }
    return 0;
}
