/*****************************************************************//**
 * \file   DllMain.cc
 * \brief  
 * 
 * \author ALPEREN
 * \date   May 2020 
 * <br>
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
#include <iostream>
#include "Hooks/Hooks.hpp"

static_assert(sizeof(uintptr_t) == 4);

#define INVALID_HANDLE(x) (nullptr == (x) || INVALID_HANDLE_VALUE == (x))

void GatewayFunction(void); // Main gateway function EndScene Mid-Function hooking ( look: Gateway.cc)

/* GLOBALS
   Don't use smart pointers! they are slow 
*/
MidFunctionHook* pMidHook;


/**
 * \brief Application main thread
 * 
 * \param hDll HINSTANCE to dll
 */
DWORD 
WINAPI 
MainThread(HMODULE hDll)
{
    pMidHook = new MidFunctionHook();


    if (nullptr != pMidHook) 
        delete pMidHook; // Runs dehook 

    FreeLibraryAndExitThread(hDll, 0); // We are done
    return TRUE;
}

/**
 * \brief Dll entry point 
 * 
 * \param hDll
 * \param dwReasonCall
 * \param lpReserved
 * \return 
 */
BOOL 
WINAPI 
DllMain(HMODULE hDll, DWORD dwReasonCall, LPVOID lpReserved)
{
    switch(dwReasonCall) {
        case DLL_PROCESS_ATTACH:
            HANDLE hMainThread = CreateThread(nullptr, 0,
                reinterpret_cast<LPTHREAD_START_ROUTINE>(MainThread), hDll, 0, nullptr);
            if (!INVALID_HANDLE(hMainThread))
                CloseHandle(hMainThread);
        break;
    }
    return TRUE;
}