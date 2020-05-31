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
#include "Strings.hpp"
#include <string>


uintptr_t _netvarOffsets[netvarOffsetsLength] = { 0 };


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


    return true;


}

/**
 * \brief Returns netvar offset
 * 
 * \param netVar enum NetvarNames
 */
inline
uintptr_t 
GetNetvarOffset(uintptr_t netVar)
{
    return _netvarOffsets[netVar];
}
