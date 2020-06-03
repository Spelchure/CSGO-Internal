/*****************************************************************//**
 * \file   EntityPlayer.hpp
 * \brief  
 * 
 * 
 * \author ALPEREN
 * \date   June 2020
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
#include "Netvars.hpp"
#include "Vectors.hpp"

extern uintptr_t _netvarOffsets[netvarOffsetsLength];
extern uint16_t sig_bDormant;


/*
 * \brief Returns netvar offset
 *
 * \param netVar enum NetvarNames
 */
inline
uintptr_t
netvar(uintptr_t netVar)
{
    return _netvarOffsets[netVar];
}

class CEntityPlayer {
public:
    int * GetHealth() {
        return (int*)((uintptr_t)this + netvar(m_iHealth));
    }
    int * GetTeam() {
        return (int*)((uintptr_t)this + netvar(m_iTeamNum));
    }
    bool *GetDormant() {
        return (bool*)((uintptr_t)this + sig_bDormant);
    }
    Vector * GetVecOrigin() {
        return (Vector*)((uintptr_t)this + netvar(m_vecOrigin));
    }
    Vector * GetViewOffset(){
        return (Vector*)((uintptr_t)this + netvar(m_vecViewOffset));
    }
    Vector * GetBonePos(int boneID) {
        uint32_t boneMatrix = *(uintptr_t*)((uintptr_t)this + netvar(m_dwBoneMatrix));
        static Vector3 bonePos;
        bonePos.x = *(float*)(boneMatrix + 0x30 * boneID + 0x0C);
        bonePos.y = *(float*)(boneMatrix + 0x30 * boneID + 0x1C);
        bonePos.z = *(float*)(boneMatrix + 0x30 * boneID + 0x2C);
        return &bonePos;
    }
    Vector2 * GetAimPunchAngles() const {
        return (Vector2*)((uintptr_t)this + netvar(m_aimPunchAngle));
    }
    void GetViewAngles(Vector &ang) {
        Vector vecOrigin = *this->GetVecOrigin();
        ang = vecOrigin + *this->GetViewOffset();
    }
};
