/*****************************************************************//**
 * \file   Interfaces.cpp
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
#include "Interfaces.hpp"
#include "Strings.hpp"
#include "TraceRay.hpp"
#include <Windows.h>


/**
 * \brief Interface definitions \sa SDK_initInterfaces()
 */
IBaseClientDLL * iClientDll; 
IClientEntityList* iEntityList;
IVEngineClient013* iEngineClient13;
IEngineTrace* iTrace;   // defined in TraceRay.hpp

/**
 * Calls cs:go CreateInterface function
 * 
 * \param dllName Name of dll (client or engine)
 * \param interfaceName Name of the interface \sa Strings.hpp
 * \returns Created interface or nullptr 
 */
void* 
GetInterface(const char* dllName, const char* interfaceName)
{
    
    if (nullptr == dllName || interfaceName == nullptr)
        return nullptr;

    tCreateInterface pCreateInterface;
    pCreateInterface = reinterpret_cast<tCreateInterface>(GetProcAddress(GetModuleHandleA(dllName), "CreateInterface"));
    if (nullptr == pCreateInterface)
        return nullptr;
    
    void* addy = pCreateInterface(interfaceName, nullptr);

    return addy;
}

/**
 * Gets SDK interfaces using GetInterface()  
 * 
 * \return True on success 
 */
bool 
SDK_initInterfaces(void)
{
    iEntityList = reinterpret_cast<IClientEntityList*>(GetInterface(DLL_CLIENT, VCLIENTENTITYLIST_INTERFACE_VERSION));
    if (nullptr == iEntityList)
        return false;

    iEngineClient13 = reinterpret_cast<IVEngineClient013*>(GetInterface(DLL_ENGINE, VENGINE_CLIENT_INTERFACE_VERSION));
    if (nullptr == iEngineClient13)
        return false;

    iTrace = reinterpret_cast<IEngineTrace*>(GetInterface(DLL_ENGINE, ENGINE_TRACE_CLIENT_VERSION));
    if (nullptr == iTrace)
        return false;   

    return true;
}


