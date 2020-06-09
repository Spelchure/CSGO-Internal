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


#define IS_VALID_ENT(ent) (((*(ent)->GetHealth()) > 0   && !(*(ent)->GetDormant())))

constexpr auto ENTITY_MAX_PLAYERS = 32;

extern AppSettings* pSettings;
extern IVEngineClient013* iEngineClient13;
extern IClientEntityList* iEntityList;
extern IEngineTrace* iTrace;

extern uintptr_t sig_dwForceAttack;
extern uintptr_t sig_dwClientState;
extern uint16_t sig_dwClientState_ViewAngles;
extern uintptr_t sig_dwViewMatrix;
extern uintptr_t sig_dwForceJump;
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

    if (!iEngineClient13->IsInGame()) // Player not in game
        return;

    int j = iEngineClient13->GetLocalPlayer();
    localPlayer = (CEntityPlayer*)iEntityList->GetClientEntity(j);
    float distmax = FLT_MAX;
    int bestEnemy = -1;
    uintptr_t clientState = 0;

    if (localPlayer == nullptr) // We do not need run any hacks
        return;

    if (sig_dwClientState)
        clientState = *(uintptr_t*)sig_dwClientState;
 
    // Copy view matrix
    float viewMatrixCopy[16] = { 0 };
    memcpy(&viewMatrixCopy, (PBYTE)sig_dwViewMatrix, sizeof(viewMatrixCopy));
    
    int numberOfEntites = iEntityList->NumberOfEntities(false);

    // Loop in entity list
    for(int i = 0; i < numberOfEntites; i++) {
        if (i == j) // Skip local
            continue;
        
        CEntityPlayer* ent = (CEntityPlayer*)iEntityList->GetClientEntity(i);
        
        if (ent == nullptr) // Not valid entity
            continue;

        if (ent == localPlayer) // Skip local player
            continue;
   
        if (!ent->IsPlayer()) // Entity is not player
            continue;

        if (*ent->GetDormant()) // 
            continue;

        if(pSettings->getBool(pSettings->bAimbot)) { // check this.

            if (*ent->GetHealth() > 0 && *ent->GetTeam() != *localPlayer->GetTeam() &&
                enemyIsVisible(ent) // Visible for us
                ) { // VALID ENEMY
                Vector myPos;
                Vector angles;
                Vector* viewAngles = (Vector*)(clientState + sig_dwClientState_ViewAngles);
                localPlayer->GetViewAngles(myPos);
                angles = vecCalcAngles(myPos, *ent->GetBonePos(8));
                float dist = vecGet3DDistance(*viewAngles, angles);
                if (dist < distmax) {
                    distmax = dist;
                    if (pSettings->getBool(pSettings->bAimbot_FOV)) {
                        if (dist <= pSettings->getFloat(pSettings->fAimFOV)) // Aim fov
                            bestEnemy = i;
                    }
                    else
                        bestEnemy = i;
                }
            }
        }
  
        
  //      if (!IS_VALID_ENT(ent))
   //         continue;

        /* Drawing Stuff */
        if (*ent->GetHealth() > 0) // if enemys health > 0
        {
            int my_team = *localPlayer->GetTeam();
            int en_team = *ent->GetTeam();

            if (my_team == en_team) // teammate
            {
                if (pSettings->getBool(pSettings->bESP_mate))
                {
                    //mate esp
                    ent->DrawESP(viewMatrixCopy, D3DCOLOR_ARGB(255, 0, 255, 0));
                }
                if (pSettings->getBool(pSettings->bSnaplines_mate))
                {
                    //snaplines mate
                    ent->DrawSnapline(viewMatrixCopy, 2, D3DCOLOR_ARGB(255, 0, 255, 0));
                }
                //maybe glow
            }
            else // enemy 
            {
                if (pSettings->getBool(pSettings->bESP))
                {
                    //enemy esp
                    ent->DrawESP(viewMatrixCopy, D3DCOLOR_ARGB(255, 255, 0, 0));
                }
                if (pSettings->getBool(pSettings->bSnaplines))
                {
                    //enemy snaplines     
                    ent->DrawSnapline(viewMatrixCopy, 2, D3DCOLOR_ARGB(255, 255, 0, 0));
                }
            }
        }
        
        

    }

    if (pSettings->getBool(pSettings->bAntiflash) && *localPlayer->GetFlashDuration() > 0) // antiflash
        *localPlayer->GetFlashDuration() = 0;

    // assuming sig_dwForceJump accessible
    if (pSettings->getBool(pSettings->bBunnyhop)) // bunnyhop
    {
        if(GetAsyncKeyState(VK_SPACE)) {
            if (localPlayer->GetFlags() & (1 << 0)) // on ground 
                *(uintptr_t*)sig_dwForceJump = 5;
            else // on air
                *(uintptr_t*)sig_dwForceJump = 4;
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
                    localPlayer->GetViewAngles(myPos); // Get eye pos
                    angles = vecCalcAngles(myPos, *enemy->GetBonePos(8));
                   
                    Vector2* aimPunchAngle = localPlayer->GetAimPunchAngles();
                   
                    angles.x -= aimPunchAngle->x * 2.f;
                    angles.y -= aimPunchAngle->y * 2.f; 
                   
                    Vector* viewAngles = (Vector*)(clientState + sig_dwClientState_ViewAngles);
                   
                    Vector relativeAngles;
                    relativeAngles.x = angles.x - viewAngles->x;
                    relativeAngles.y = angles.y - viewAngles->y;
                    
                    relativeAngles.Normalize();
                    relativeAngles.Clamp();

                    angles.x = viewAngles->x;
                    angles.y = viewAngles->y;

                    angles.x += relativeAngles.x / pSettings->getFloat(pSettings->fAimSmooth);
                    angles.y += relativeAngles.y / pSettings->getFloat(pSettings->fAimSmooth);

                    angles.Normalize(); angles.Clamp();

                    viewAngles->x = angles.x;
                    viewAngles->y = angles.y;

                    if(pSettings->getBool(pSettings->bAimbot_AutoAttack) && sig_dwForceAttack) {
                        *(uintptr_t*)(sig_dwForceAttack) = 5;
                        Sleep(50);
                        *(uintptr_t*)(sig_dwForceAttack) = 4;
                    }
                }
            }
        }

    }
}