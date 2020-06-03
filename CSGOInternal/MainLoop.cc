/*****************************************************************//**
 * \file   MainLoop.cc
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
#include "Settings/Settings.hpp"
#include "Source SDK/SDK.hpp"

constexpr auto ENTITY_MAX_PLAYERS = 32;

extern AppSettings* pSettings;
extern IVEngineClient013* iEngineClient13;
extern IClientEntityList* iEntityList;
extern IEngineTrace* iTrace;

extern uintptr_t sig_dwClientState;
extern uint16_t sig_dwClientState_ViewAngles;
CEntityPlayer* localPlayer;

/**
 * \brief Checks the enemy is visible for aimbot using TraceRay
 * 
 */
static
bool  
enemyIsVisible(CEntityPlayer* enemy)
{
    if (nullptr == localPlayer || nullptr == enemy)
        return false;
    if (iTrace == nullptr)
        return false;

    Vector eyepos;
    localPlayer->GetViewAngles(eyepos);
    Vector* enemyBone = enemy->GetBonePos(8);


    CGameTrace trace;
    Ray_t ray;
    CTraceFilter tracefilter;

    tracefilter.pSkip = (void*)localPlayer;

    ray.Init(eyepos, *enemyBone);

    iTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &tracefilter, &trace);

    if (enemy == trace.hit_entity)
        return true;

    return false;
}

void 
MainLoop(void)
{
    localPlayer = iEntityList->GetClientEntity(iEngineClient13->GetLocalPlayer());
    float distmax = FLT_MAX;
    int bestEnemy = -1;
    uintptr_t clientState = 0;

    if (localPlayer == nullptr) // We do not need run any hacks
        return;

    if (sig_dwClientState)
        clientState = *(uintptr_t*)sig_dwClientState;
  
    // Loop in entity list
    for(int i = 0; i < ENTITY_MAX_PLAYERS; i++) {
        CEntityPlayer* ent = iEntityList->GetClientEntity(i);
        
        if (ent == nullptr) // Reached end of the list
            break;
        
        if (ent == localPlayer) // Skip local player
            continue;
    

        if (*ent->GetHealth() > 0 && *ent->GetTeam() != *localPlayer->GetTeam() && !(*ent->GetDormant()) &&  
            enemyIsVisible(ent) // Visible for us
            ) { // VALID ENEMY
            if (true) { // IsAimbot open ? 
                Vector myPos;
                Vector angles;

                localPlayer->GetViewAngles(myPos);
                angles = vecCalcAngles(myPos, *ent->GetBonePos(8));
            
                float dist = vecGet3DDistance(angles, *ent->GetBonePos(8));
                if (dist < distmax)
                    bestEnemy = i;
            }
        }
    }

    if (GetAsyncKeyState('C') & 0x8000) { // Aim!! 
        if(bestEnemy > -1) {
            //Aim to enemy 
            Vector myPos, angles;
            CEntityPlayer* enemy = (CEntityPlayer*)iEntityList->GetClientEntity(bestEnemy);
            if (nullptr != enemy)
            {
                
                if(clientState) {
                    localPlayer->GetViewAngles(myPos);
                    angles = vecCalcAngles(myPos, *enemy->GetBonePos(8));
                    Vector2* aimPunchAngle = localPlayer->GetAimPunchAngles();
                    
                    angles.x -= aimPunchAngle->x * 2.f;
                    angles.y -= aimPunchAngle->y * 2.f; 
                                      
                    if(angles.x >= -89.0f && angles.x <= 89.0f && angles.y >= -180.0f && angles.y <= 180.0f) {
                        Vector* viewAngles = (Vector*)(clientState + sig_dwClientState_ViewAngles);
                        //Set my angles...
                        viewAngles->x = angles.x;
                        viewAngles->y = angles.y;
                    }
                    
                }
            }
        }

    }
}