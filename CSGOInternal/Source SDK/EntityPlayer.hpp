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
#include "../D3D9/Drawing.hpp"

#pragma warning (disable : 4244)

extern uintptr_t _netvarOffsets[netvarOffsetsLength];
extern uint16_t sig_bDormant;
extern D3D9Drawing* pDrawing;

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
    int8_t GetFlags() {
        return *(int8_t*)((uintptr_t)this + netvar(m_fFlags));
    }
    int *GetFlashDuration() {
        return (int*)((uintptr_t)this + netvar(m_flFlashDuration));
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
    /* DRAWING */

    void DrawESP(float* v, D3DCOLOR color)
    {
        Vector vHead = *this->GetBonePos(8);
        vHead.z += 8;
        Vector vOrigin = *this->GetVecOrigin();
        Vector2 vScreen, vHead2;
        
        if(pDrawing->World2Screen(v,vOrigin, vScreen)) {
            if(pDrawing->World2Screen(v,vHead,vHead2)) {
                pDrawing->DrawEspBox(vScreen, vHead2, 2, color);
            }
        }
    }

    void DrawSnapline(float* v, int thick, D3DCOLOR color)
    {
        Vector vOrigin = *this->GetVecOrigin();
        Vector2 entPos;

        if(pDrawing->World2Screen(v,vOrigin,entPos)) 
            pDrawing->DrawLine(entPos.x, entPos.y, pDrawing->screen_w / 2, pDrawing->screen_h , thick, color);
        
    }

};
