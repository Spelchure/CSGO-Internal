/*****************************************************************//**
 * \file   Interfaces.hpp
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


using tCreateInterface = void* (__cdecl*)(const char* interfaceName, int * returnCode);

void* GetInterface(const char *dllName, const char* interfaceName);

typedef void* CBaseHandle;
typedef void* INetChannel;
typedef void* IServer;
typedef void* CGlobalVarsBase;

class ClientClass;

class ClientEntity {

};

class IBaseClientDLL
{
public:
    // Called once when the client DLL is loaded
    virtual int				Init(tCreateInterface appSystemFactory,
        tCreateInterface physicsFactory,
        CGlobalVarsBase* pGlobals) = 0;

    virtual void			PostInit() = 0;

    // Called once when the client DLL is being unloaded
    virtual void			Shutdown(void) = 0;

    // Called once the client is initialized to setup client-side replay interface pointers
    virtual bool			ReplayInit(tCreateInterface replayFactory) = 0;
    virtual bool			ReplayPostInit() = 0;

    // Called at the start of each level change
    virtual void			LevelInitPreEntity(char const* pMapName) = 0;
    // Called at the start of a new level, after the entities have been received and created
    virtual void			LevelInitPostEntity() = 0;
    // Called at the end of a level
    virtual void			LevelShutdown(void) = 0;

    // Request a pointer to the list of client datatable classes
    virtual ClientClass* GetAllClasses(void) = 0;

    // Called once per level to re-initialize any hud element drawing stuff
    virtual int				HudVidInit(void) = 0;
    // Called by the engine when gathering user input
    virtual void			HudProcessInput(bool bActive) = 0;
    // Called oncer per frame to allow the hud elements to think
    virtual void			HudUpdate(bool bActive) = 0;
    // Reset the hud elements to their initial states
    virtual void			HudReset(void) = 0;
    // Display a hud text message
    virtual void			HudText(const char* message) = 0;

    // Mouse Input Interfaces
    // Activate the mouse (hides the cursor and locks it to the center of the screen)
    virtual void			IN_ActivateMouse(void) = 0;
    // Deactivates the mouse (shows the cursor and unlocks it)
    virtual void			IN_DeactivateMouse(void) = 0;
};

/**
 * Interface IClientEntityList
 */
class IClientEntityList
{
public:
    // Get IClientNetworkable interface for specified entity
    virtual void* GetClientNetworkable(int entnum) = 0;
    virtual void* GetClientNetworkableFromHandle(CBaseHandle hEnt) = 0;
    virtual void* GetClientUnknownFromHandle(CBaseHandle hEnt) = 0;

    // NOTE: This function is only a convenience wrapper.
    // It returns GetClientNetworkable( entnum )->GetIClientEntity().
    virtual ClientEntity* GetClientEntity(int entnum) = 0;
    virtual ClientEntity* GetClientEntityFromHandle(CBaseHandle hEnt) = 0;

    // Returns number of entities currently in use
    virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;

    // Returns highest index actually used
    virtual int					GetHighestEntityIndex(void) = 0;

    // Sizes entity list to specified size
    virtual void				SetMaxEntities(int maxents) = 0;
    virtual int					GetMaxEntities() = 0;
};


