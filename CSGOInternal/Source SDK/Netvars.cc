/*****************************************************************//**
 * \file   Netvars.cc
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
#include "Netvars.hpp"
#include "Interfaces.hpp"
#include "../Hooks/Memory.hpp"
#include "Strings.hpp"
#include <Windows.h>
#include <string>


uintptr_t _netvarOffsets[netvarOffsetsLength] = { 0 };
uintptr_t sig_dwClientState = 0;
uintptr_t sig_dwppDirect3DDevice9 = 0;
uint16_t sig_dwClientState_MaxPlayers = 0;
uint16_t sig_dwClientState_ViewAngles = 0;
uint16_t sig_bDormant = 0;
uintptr_t sig_dwForceAttack = 0;
uintptr_t sig_dwForceJump = 0;
uintptr_t sig_dwViewMatrix = 0;


/**
 * \brief Returns network variable offset
 * 
 */
static
uintptr_t 
_GetOffset(RecvTable* table, const char* tableName, const char* netvarName)
{
    for (int i = 0; i < table->m_nProps; i++)
    {
        RecvProp prop = table->m_pProps[i];

        if (!_stricmp(prop.m_pVarName, netvarName))
            return prop.m_Offset;

        if (prop.m_pDataTable)
        {
            uintptr_t offset = _GetOffset(prop.m_pDataTable, tableName, netvarName);
            if (offset)
                return offset + prop.m_Offset;
        }
    }

    return 0;
}


uintptr_t 
_GetNetVarOffset(const char* tableName, const char* netvarName, ClientClass* clientClass)
{
    ClientClass* currNode = clientClass;

    for (auto currNode = clientClass; currNode; currNode = currNode->m_pNext)
    {
        if (!_stricmp(tableName, currNode->m_pRecvTable->m_pNetTableName))
        {
            return _GetOffset(currNode->m_pRecvTable, tableName, netvarName);
        }
    }
    return 0;
}


/**
 * \brief Initialize netvars
 * 
 */
bool 
ReadyNetvarOffsets(void)
{
    ClientClass* allClass;
    IBaseClientDLL* baseClientDLL = reinterpret_cast<IBaseClientDLL*>(GetInterface(DLL_CLIENT, CLIENT_DLL_INTERFACE_VERSION));

    if (baseClientDLL == nullptr)
        return false;

    allClass = baseClientDLL->GetAllClasses();
    if (nullptr == allClass)
        return false;

    
    _netvarOffsets[NetvarNames::m_iHealth] = _GetNetVarOffset("DT_BasePlayer", "m_iHealth", allClass);
    _netvarOffsets[NetvarNames::m_iTeamNum] = _GetNetVarOffset("DT_BasePlayer", "m_iTeamNum", allClass);
    _netvarOffsets[NetvarNames::m_vecOrigin] = _GetNetVarOffset("DT_BasePlayer", "m_vecOrigin", allClass);
    _netvarOffsets[NetvarNames::m_vecVelocity] = _GetNetVarOffset("DT_CSPlayer", "m_vecVelocity[0]", allClass);
    _netvarOffsets[NetvarNames::m_vecViewOffset] = _GetNetVarOffset("DT_CSPlayer", "m_vecViewOffset[0]", allClass);
    _netvarOffsets[NetvarNames::m_viewPunchAngle] = _GetNetVarOffset("DT_BasePlayer", "m_viewPunchAngle", allClass);
    _netvarOffsets[NetvarNames::m_hMyWeapons] = _GetNetVarOffset("DT_BasePlayer", "m_hMyWeapons", allClass);
    _netvarOffsets[NetvarNames::m_flFlashDuration] = _GetNetVarOffset("DT_CSPlayer", "m_flFlashDuration", allClass);
    _netvarOffsets[NetvarNames::m_bIsDefusing] = _GetNetVarOffset("DT_CSPlayer", "m_bIsDefusing", allClass);
    _netvarOffsets[NetvarNames::m_aimPunchAngle] = _GetNetVarOffset("DT_BasePlayer", "m_aimPunchAngle", allClass);
    _netvarOffsets[NetvarNames::m_fFlags] = _GetNetVarOffset("DT_CSPlayer", "m_fFlags", allClass);
    _netvarOffsets[NetvarNames::m_dwBoneMatrix] = _GetNetVarOffset("DT_BaseAnimating", "m_nForceBone", allClass); 
    _netvarOffsets[NetvarNames::m_dwBoneMatrix] += 28;


    return true;
}

template <typename T>
inline
T
read_protected_memory(byte_t* v)
{
    DWORD dwOldProtect;
    VirtualProtect(v, sizeof(T), PAGE_EXECUTE_READWRITE, &dwOldProtect);
    T val = *(T*)(v);
    VirtualProtect(v, sizeof(T), dwOldProtect, &dwOldProtect);
    return val;
}

bool 
ReadySignatures(void)
{
    byte_t* mem;
    //Currently we are using hazedumper signatures.
   // return true;

    mem = (byte_t*) memFindPattern(DLL_ENGINE, PATTERN_CLIENTSTATE[0], PATTERN_CLIENTSTATE[1]);
    if (!mem)
        return false;

    sig_dwClientState = read_protected_memory<uintptr_t>(++mem);

    mem = (byte_t*) memFindPattern(DLL_ENGINE, PATTERN_CLIENTSTATE_MAXPLAYERS[0], PATTERN_CLIENTSTATE_MAXPLAYERS[1]);
    if (!mem)
        return false;

    mem += 7;
    sig_dwClientState_MaxPlayers = read_protected_memory<uint16_t>(mem);


    mem = (byte_t*) memFindPattern(DLL_ENGINE, PATTERN_CLIENTSTATE_VIEWANGLES[0], PATTERN_CLIENTSTATE_VIEWANGLES[1]);
    if (!mem)
        return false;
    mem += 4;
    sig_dwClientState_ViewAngles = read_protected_memory<uint16_t>(mem);

    
    mem = (byte_t*) memFindPattern(DLL_CLIENT, PATTERN_BDORMANT[0], PATTERN_BDORMANT[1]);
    if (!mem)
        return false;
    mem += 2;
    sig_bDormant = read_protected_memory<uint16_t>(mem);
    sig_bDormant += 8;

    mem = (byte_t*)memFindPattern(DLL_SHADERAPI, PATTERN_PTRD3DDEV[0], PATTERN_PTRD3DDEV[1]);
    if (!mem)
        return false;
    sig_dwppDirect3DDevice9 = read_protected_memory<uintptr_t>(++mem);

    mem = (byte_t*)memFindPattern(DLL_CLIENT, PATTERN_VIEWMATRIX[0], PATTERN_VIEWMATRIX[1]);
    if (!mem)
        return false;
    mem += 3;
    sig_dwViewMatrix = read_protected_memory<uintptr_t>(mem);
    sig_dwViewMatrix += 0xB0;
   

    mem = (byte_t*)memFindPattern(DLL_CLIENT, PATTERN_FORCEJUMP[0], PATTERN_FORCEJUMP[1]);
    if (!mem)
        return false;
    mem += 2;
    sig_dwForceJump = read_protected_memory<uintptr_t>(mem);

    // Not required don't crash !!
    mem = (byte_t*)memFindPattern(DLL_CLIENT, PATTERN_FORCEATTACK[0], PATTERN_FORCEATTACK[1]);
    if (mem)
    {
        mem += 2;
        sig_dwForceAttack = read_protected_memory<uintptr_t>(mem);
    }
   
    return true;
}
