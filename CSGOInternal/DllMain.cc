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
#include <Shlwapi.h>
#include <iostream>
#include <string>
#include <sstream>
#include "Keys.hpp"
#include "MainMenu.hpp"
#include "Settings/Settings.hpp"
#include "Hooks/Hooks.hpp"
#include "D3D9/DummyDevice.hpp"
#include "Source SDK/SDK.hpp"

#pragma comment(lib, "Shlwapi.lib")

#define INVALID_HANDLE(x) (nullptr == (x) || INVALID_HANDLE_VALUE == (x))

static_assert(sizeof(uintptr_t) == 4);

void GatewayFunction(void); // Main gateway function EndScene Mid-Function hooking ( Gateway.cc)

constexpr auto SETTINGS_FILE_NAME = "SimpleSettings.cfg";

/* GLOBALS
   Don't use smart pointers! they are slow 
*/
MidFunctionHook* pMidHook;
AppSettings* pSettings;
MainMenu* pMenu;
void* vTable[119];

extern uintptr_t sig_dwppDirect3DDevice9;

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
    pSettings = new AppSettings();
    pMenu = new MainMenu();

    // Find CS:GO Window
    HWND hValveWindow = FindWindowA(VALVE_WINDOW_CLASS, 0);
    if(nullptr == hValveWindow){
        delete pMenu;
        delete pMidHook;
        delete pSettings;
        FreeLibraryAndExitThread(hDll, 1);
        return FALSE;
    }

    // Initialize settings & Read settings file 'SETTINGS_FILE_NAME'
    char* dllPath = new char[_MAX_PATH];
    if(GetModuleFileNameA(hDll,dllPath, _MAX_PATH)) {
        PathRemoveFileSpecA(dllPath);
        std::stringstream ss(dllPath, std::ios_base::app | std::ios_base::out);
    
        ss << "\\"  << SETTINGS_FILE_NAME;
         
        if(!pSettings->parseFile(ss.str().c_str())) 
            pSettings->setDefault();
        
    } else {
        pSettings->setDefault(); // Set default settings
    }
   
    if (dllPath)
        delete[] dllPath;


    // Get d3d9 device for address of EndScene
    if (!GetD3D9Device(hValveWindow, vTable, sizeof(vTable)))
    {
        
        delete pMenu;
        delete pMidHook;
        delete pSettings;
        FreeLibraryAndExitThread(hDll, 1);
        return FALSE;
    }


    // Initialize netvar offsets
    if(!ReadyNetvarOffsets()) {
        
        delete pMenu;
        delete pMidHook;
        delete pSettings;
        FreeLibraryAndExitThread(hDll, 1);
        return FALSE;
    }
    
    // Initialize signatures
    if(!ReadySignatures()) {

        delete pMenu;
        delete pMidHook;
        delete pSettings;
        FreeLibraryAndExitThread(hDll, 1);
        return FALSE;
    }
    
    if (!SDK_initInterfaces())
    {
        delete pMenu;
        delete pMidHook;
        delete pSettings;
        FreeLibraryAndExitThread(hDll, 1);
        return FALSE;
    }
    
    LPDIRECT3DDEVICE9 pDev = reinterpret_cast<LPDIRECT3DDEVICE9>(*(uintptr_t*)sig_dwppDirect3DDevice9);
    if(nullptr == pDev) {
        delete pMenu;
        delete pMidHook;
        delete pSettings;
        FreeLibraryAndExitThread(hDll, 1);
        return FALSE;
    }
    if (!pMenu->InitMenu(pDev, hValveWindow)) {
        delete pMenu;
        delete pMidHook;
        delete pSettings;
        FreeLibraryAndExitThread(hDll, 1);
        return FALSE;
    }
    
    // HOOK! GatewayFunction:Gateway.cc
    pMidHook->BeginHook((reinterpret_cast<uintptr_t>(vTable[42])+0xF), (uintptr_t)GatewayFunction, 5);
    
    while (!GetAsyncKeyState(KEYS_EXIT))
    {
        if (GetAsyncKeyState(KEYS_TOGGLE) & 1)
            pSettings->toggleBool(pSettings->bMenuShowing);
    }
    
    //Save settings
    pSettings->saveSettings();
    pSettings->setBool(pSettings->bMenuShowing, false);

    if (nullptr != pMidHook) 
        delete pMidHook; // Runs dehook 

    if (nullptr != pSettings)
        delete pSettings;

    if (nullptr != pMenu)
        delete pMenu;


    FreeLibraryAndExitThread(hDll, 0); // We are done
    return TRUE;
}

/**
 * \brief Dll entry point 
 * 
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