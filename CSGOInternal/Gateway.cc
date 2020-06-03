/*****************************************************************//**
 * \file   Gateway.cc
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
#include "Hooks/Hooks.hpp"
#include "Settings/Settings.hpp"
#include "MainMenu.hpp"
extern MidFunctionHook* pMidHook;
extern AppSettings* pSettings;
extern MainMenu* pMenu;

void MainLoop(void);

/**
 * \brief Hooked Direct3D EndScene function 
 *
 * __declspec(naked) : we have function's epilogue and prologue. 
 */
__declspec(naked)
void 
GatewayFunction(void)
{
    __asm pushad // Push all registers to stack 
    static uintptr_t jmpBack = pMidHook->hookJumpBack;
                 
    // Menu drawing 
    if (pSettings->getBool(pSettings->bMenuShowing)) // Show menu 
        pMenu->PrintMenu();

    // call MainLoop
    MainLoop();

    __asm popad // Pop all
   
    /*
        This instructions must be same pMidHook->originalBytes 
    */
    __asm {
        mov ebx, edi
        lea eax, [edi + 04]
        jmp [jmpBack];
    }
    
}
