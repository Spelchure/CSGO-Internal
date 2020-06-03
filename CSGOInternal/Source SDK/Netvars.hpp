/*****************************************************************//**
 * \file   Netvars.hpp
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
#include <cstdint>



enum NetvarNames {
    m_iHealth,
    m_iTeamNum,
    m_vecOrigin,
    m_vecVelocity,
    m_vecViewOffset,
    m_viewPunchAngle,
    m_hMyWeapons,
    m_flFlashDuration,
    m_bIsDefusing,
    m_dwBoneMatrix
};
constexpr auto netvarOffsetsLength = 10;

class RecvTable;

class RecvProp
{
public:
    char*                    m_pVarName;
    void*                   m_RecvType;
    int                     m_Flags;
    int                     m_StringBufferSize;
    int                     m_bInsideArray;
    const void*             m_pExtraData;
    RecvProp*               m_pArrayProp;
    void*                   m_ArrayLengthProxy;
    void*                   m_ProxyFn;
    void*                   m_DataTableProxyFn;
    RecvTable*              m_pDataTable;
    int                     m_Offset;
    int                     m_ElementStride;
    int                     m_nElements;
    const char*             m_pParentArrayPropName;
};

class RecvTable
{
public:

    RecvProp*           m_pProps;
    int                 m_nProps;
    void*               m_pDecoder;
    char*               m_pNetTableName;
    bool                m_bInitialized;
    bool                m_bInMainList;
};

class ClientClass
{
public:
    void*            m_pCreateFn;
    void*            m_pCreateEventFn;
    char*            m_pNetworkName;
    RecvTable*       m_pRecvTable;
    ClientClass*     m_pNext;
    int              m_ClassID;
};

//uintptr_t _GetOffset(RecvTable* table, const char* tableName, const char* netvarName);
uintptr_t _GetNetVarOffset(const char* tableName, const char* netvarName, ClientClass* clientClass);
bool ReadyNetvarOffsets(void);
bool ReadySignatures(void);
