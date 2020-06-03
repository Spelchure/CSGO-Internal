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
#include "EntityPlayer.hpp"

using tCreateInterface = void* (__cdecl*)(const char* interfaceName, int * returnCode);


typedef void* CBaseHandle;
typedef void* INetChannel;
typedef void* IServer;
typedef void* CGlobalVarsBase;
typedef int player_info_t;
typedef char client_textmessage_t;

class ClientClass;




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
    virtual CEntityPlayer* GetClientEntity(int entnum) = 0;
    virtual CEntityPlayer* GetClientEntityFromHandle(CBaseHandle hEnt) = 0;

    // Returns number of entities currently in use
    virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;

    // Returns highest index actually used
    virtual int					GetHighestEntityIndex(void) = 0;

    // Sizes entity list to specified size
    virtual void				SetMaxEntities(int maxents) = 0;
    virtual int					GetMaxEntities() = 0;
};

class IVEngineClient013
{
public:
    // Find the model's surfaces that intersect the given sphere.
    // Returns the number of surfaces filled in.
    virtual void fn0(void) = 0;
    // Get the lighting intensivty for a specified point
    // If bClamp is specified, the resulting Vector is restricted to the 0.0 to 1.0 for each element
    virtual void fn1(void) = 0;
    // Traces the line and reports the material impacted as well as the lighting information for the impact point

    virtual void fn2(void) = 0;
    // Given an input text buffer data pointer, parses a single token into the variable token and returns the new
    //  reading position
    virtual const char*         ParseFile(const char* data, char* token, int maxlen) = 0;
    virtual bool				CopyLocalFile(const char* source, const char* destination) = 0;

    // Gets the dimensions of the game window
    virtual void				GetScreenSize(int& width, int& height) = 0;

    // Forwards szCmdString to the server, sent reliably if bReliable is set
    virtual void				ServerCmd(const char* szCmdString, bool bReliable = true) = 0;
    // Inserts szCmdString into the command buffer as if it was typed by the client to his/her console.
    // Note: Calls to this are checked against FCVAR_CLIENTCMD_CAN_EXECUTE (if that bit is not set, then this function can't change it).
    //       Call ClientCmd_Unrestricted to have access to FCVAR_CLIENTCMD_CAN_EXECUTE vars.
    virtual void				ClientCmd(const char* szCmdString) = 0;

    // Fill in the player info structure for the specified player index (name, model, etc.)
    virtual bool				GetPlayerInfo(int ent_num, player_info_t* pinfo) = 0;

    // Retrieve the player entity number for a specified userID
    virtual int					GetPlayerForUserID(int userID) = 0;

    // Retrieves text message system information for the specified message by name
    virtual client_textmessage_t* TextMessageGet(const char* pName) = 0;

    // Returns true if the console is visible
    virtual bool				Con_IsVisible(void) = 0;

    // Get the entity index of the local player
    virtual int					GetLocalPlayer(void) = 0;
};

void* GetInterface(const char *dllName, const char* interfaceName);
bool SDK_initInterfaces(void);

