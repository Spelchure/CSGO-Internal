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

typedef void* ShouldTransmitState_t;
typedef void* DataUpdateType_t;
typedef void* ClientShadowHandle_t;
typedef void* ClientRenderHandle_t;
typedef void* IPVSNotify;
typedef void* ModelInstanceHandle_t;
typedef void* SpatializationInfo_t;
typedef int ShadowType_t;
typedef int bf_read;
typedef void* CBaseHandle;
typedef void* INetChannel;
typedef void* IServer;
typedef void* CGlobalVarsBase;
typedef void* model_t;
typedef void* CSentence;
typedef float* QAngle;
typedef void* CAudioSource; 
typedef void* ButtonCode_t;
typedef int player_info_t;
typedef char client_textmessage_t;
typedef float* matrix3x4_t;

class IClientEntity;
class ClientClass;
class CMouthInfo;
class ICollideable;
class C_BaseEntity;
class IClientUnknown;
class IClientThinkable;
class IHandleEntity;
class IClientRenderable;

class IHandleEntity
{
public:
    virtual ~IHandleEntity() {}
    virtual void SetRefEHandle(const CBaseHandle& handle) = 0;
    virtual const CBaseHandle& GetRefEHandle() const = 0;
};

class IClientNetworkable
{
public:
    virtual IClientUnknown* GetIClientUnknown() = 0;

    // Called by the engine when the server deletes the entity.
    virtual void			Release() = 0;

    // Supplied automatically by the IMPLEMENT_CLIENTCLASS macros.
    virtual ClientClass* GetClientClass() = 0;

    // This tells the entity what the server says for ShouldTransmit on this entity.
    // Note: This used to be EntityEnteredPVS/EntityRemainedInPVS/EntityLeftPVS.
    virtual void			NotifyShouldTransmit(ShouldTransmitState_t state) = 0;



    //
    // NOTE FOR ENTITY WRITERS: 
    //
    // In 90% of the cases, you should hook OnPreDataChanged/OnDataChanged instead of 
    // PreDataUpdate/PostDataUpdate.
    //
    // The DataChanged events are only called once per frame whereas Pre/PostDataUpdate
    // are called once per packet (and sometimes multiple times per frame).
    //
    // OnDataChanged is called during simulation where entity origins are correct and 
    // attachments can be used. whereas PostDataUpdate is called while parsing packets
    // so attachments and other entity origins may not be valid yet.
    //

    virtual void			OnPreDataChanged(DataUpdateType_t updateType) = 0;
    virtual void			OnDataChanged(DataUpdateType_t updateType) = 0;

    // Called when data is being updated across the network.
    // Only low-level entities should need to know about these.
    virtual void			PreDataUpdate(DataUpdateType_t updateType) = 0;
    virtual void			PostDataUpdate(DataUpdateType_t updateType) = 0;


    // Objects become dormant on the client if they leave the PVS on the server.
    virtual bool			IsDormant(void) = 0;

    // Ent Index is the server handle used to reference this entity.
    // If the index is < 0, that indicates the entity is not known to the server
    virtual int				entindex(void) const = 0;

    // Server to client entity message received
    virtual void			ReceiveMessage(int classID, bf_read& msg) = 0;

    // Get the base pointer to the networked data that GetClientClass->m_pRecvTable starts at.
    // (This is usually just the "this" pointer).
    virtual void* GetDataTableBasePtr() = 0;

    // Tells the entity that it's about to be destroyed due to the client receiving
    // an uncompressed update that's caused it to destroy all entities & recreate them.
    virtual void			SetDestroyedOnRecreateEntities(void) = 0;

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


class IClientThinkable
{
public:
    // Gets at the containing class...
    virtual IClientUnknown* GetIClientUnknown() = 0;

    virtual void				ClientThink() = 0;

    // Called when you're added to the think list.
    // GetThinkHandle's return value must be initialized to INVALID_THINK_HANDLE.
    virtual void* GetThinkHandle() = 0;
    virtual void				SetThinkHandle(void* hThink) = 0;

    // Called by the client when it deletes the entity.
    virtual void				Release() = 0;
};

class IClientRenderable
{
public:
    // Gets at the containing class...
    virtual IClientUnknown* GetIClientUnknown() = 0;

    // Data accessors
    virtual Vector const& GetRenderOrigin(void) = 0;
    virtual QAngle const& GetRenderAngles(void) = 0;
    virtual bool					ShouldDraw(void) = 0;
    virtual bool					IsTransparent(void) = 0;
    virtual bool					UsesPowerOfTwoFrameBufferTexture() = 0;
    virtual bool					UsesFullFrameBufferTexture() = 0;

    virtual ClientShadowHandle_t	GetShadowHandle() const = 0;

    // Used by the leaf system to store its render handle.
    virtual ClientRenderHandle_t& RenderHandle() = 0;

    // Render baby!
    virtual const model_t* GetModel() const = 0;
    virtual int						DrawModel(int flags) = 0;

    // Get the body parameter
    virtual int		GetBody() = 0;

    // Determine alpha and blend amount for transparent objects based on render state info
    virtual void	ComputeFxBlend() = 0;
    virtual int		GetFxBlend(void) = 0;

    // Determine the color modulation amount
    virtual void	GetColorModulation(float* color) = 0;

    // Returns false if the entity shouldn't be drawn due to LOD. 
    // (NOTE: This is no longer used/supported, but kept in the vtable for backwards compat)
    virtual bool	LODTest() = 0;

    // Call this to get the current bone transforms for the model.
    // currentTime parameter will affect interpolation
    // nMaxBones specifies how many matrices pBoneToWorldOut can hold. (Should be greater than or
    // equal to studiohdr_t::numbones. Use MAXSTUDIOBONES to be safe.)
    virtual bool	SetupBones(matrix3x4_t* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) = 0;

    virtual void	SetupWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights) = 0;
    virtual void	DoAnimationEvents(void) = 0;

    // Return this if you want PVS notifications. See IPVSNotify for more info.	
    // Note: you must always return the same value from this function. If you don't,
    // undefined things will occur, and they won't be good.
    virtual IPVSNotify* GetPVSNotifyInterface() = 0;

    // Returns the bounds relative to the origin (render bounds)
    virtual void	GetRenderBounds(Vector& mins, Vector& maxs) = 0;

    // returns the bounds as an AABB in worldspace
    virtual void	GetRenderBoundsWorldspace(Vector& mins, Vector& maxs) = 0;

    // These normally call through to GetRenderAngles/GetRenderBounds, but some entities custom implement them.
    virtual void	GetShadowRenderBounds(Vector& mins, Vector& maxs, ShadowType_t shadowType) = 0;

    // Should this object be able to have shadows cast onto it?
    virtual bool	ShouldReceiveProjectedTextures(int flags) = 0;

    // These methods return true if we want a per-renderable shadow cast direction + distance
    virtual bool	GetShadowCastDistance(float* pDist, ShadowType_t shadowType) const = 0;
    virtual bool	GetShadowCastDirection(Vector* pDirection, ShadowType_t shadowType) const = 0;

    // Other methods related to shadow rendering
    virtual bool	IsShadowDirty() = 0;
    virtual void	MarkShadowDirty(bool bDirty) = 0;

    // Iteration over shadow hierarchy
    virtual IClientRenderable* GetShadowParent() = 0;
    virtual IClientRenderable* FirstShadowChild() = 0;
    virtual IClientRenderable* NextShadowPeer() = 0;

    // Returns the shadow cast type
    virtual ShadowType_t ShadowCastType() = 0;

    // Create/get/destroy model instance
    virtual void CreateModelInstance() = 0;
    virtual ModelInstanceHandle_t GetModelInstance() = 0;

    // Returns the transform from RenderOrigin/RenderAngles to world
    virtual const matrix3x4_t& RenderableToWorldTransform() = 0;

    // Attachments
    virtual int LookupAttachment(const char* pAttachmentName) = 0;
    virtual	bool GetAttachment(int number, Vector& origin, QAngle& angles) = 0;
    virtual bool GetAttachment(int number, matrix3x4_t& matrix) = 0;

    // Rendering clip plane, should be 4 floats, return value of NULL indicates a disabled render clip plane
    virtual float* GetRenderClipPlane(void) = 0;

    // Get the skin parameter
    virtual int		GetSkin() = 0;

    // Is this a two-pass renderable?
    virtual bool	IsTwoPass(void) = 0;

    virtual void	OnThreadedDrawSetup() = 0;

    virtual bool	UsesFlexDelayedWeights() = 0;

    virtual void	RecordToolMessage() = 0;

    virtual bool	IgnoresZBuffer(void) const = 0;
};

class IClientUnknown : public IHandleEntity
{
public:
    virtual ICollideable* GetCollideable() = 0;
    virtual IClientNetworkable* GetClientNetworkable() = 0;
    virtual IClientRenderable* GetClientRenderable() = 0;
    virtual IClientEntity* GetIClientEntity() = 0;
    virtual C_BaseEntity* GetBaseEntity() = 0;
    virtual IClientThinkable* GetClientThinkable() = 0;
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
    // Delete yourself.
    virtual void			Release(void) = 0;

    // Network origin + angles
    virtual const Vector& GetAbsOrigin(void) const = 0;
    virtual const QAngle& GetAbsAngles(void) const = 0;

    virtual CMouthInfo* GetMouth(void) = 0;

    // Retrieve sound spatialization info for the specified sound on this entity
    // Return false to indicate sound is not audible
    virtual bool			GetSoundSpatialization(SpatializationInfo_t& info) = 0;
};

class CEntityPlayer : IClientEntity {
public:
    bool IsPlayer() {
        ClientClass* cl = GetClientClass();
        if (nullptr == cl) return false;

        if (!std::string(cl->m_pNetworkName).compare("CCSPlayer"))
            return true;

        return false;
    }

    int* GetHealth() {
        return (int*)((uintptr_t)this + netvar(m_iHealth));
    }
    int* GetTeam() {
        return (int*)((uintptr_t)this + netvar(m_iTeamNum));
    }
    bool* GetDormant() {
        return (bool*)((uintptr_t)this + sig_bDormant);
    }
    int8_t GetFlags() {
        return *(int8_t*)((uintptr_t)this + netvar(m_fFlags));
    }
    int* GetFlashDuration() {
        return (int*)((uintptr_t)this + netvar(m_flFlashDuration));
    }
    Vector* GetVecOrigin() {
        return (Vector*)((uintptr_t)this + netvar(m_vecOrigin));
    }
    Vector* GetViewOffset() {
        return (Vector*)((uintptr_t)this + netvar(m_vecViewOffset));
    }
    Vector* GetBonePos(int boneID) {
        uint32_t boneMatrix = *(uintptr_t*)((uintptr_t)this + netvar(m_dwBoneMatrix));
        static Vector3 bonePos;
        bonePos.x = *(float*)(boneMatrix + 0x30 * boneID + 0x0C);
        bonePos.y = *(float*)(boneMatrix + 0x30 * boneID + 0x1C);
        bonePos.z = *(float*)(boneMatrix + 0x30 * boneID + 0x2C);
        return &bonePos;
    }
    Vector2* GetAimPunchAngles() const {
        return (Vector2*)((uintptr_t)this + netvar(m_aimPunchAngle));
    }
    void GetViewAngles(Vector& ang) {
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

        if (pDrawing->World2Screen(v, vOrigin, vScreen)) {
            if (pDrawing->World2Screen(v, vHead, vHead2)) {
                pDrawing->DrawEspBox(vScreen, vHead2, 2, color);
            }
        }
    }

    void DrawSnapline(float* v, int thick, D3DCOLOR color)
    {
        Vector vOrigin = *this->GetVecOrigin();
        Vector2 entPos;

        if (pDrawing->World2Screen(v, vOrigin, entPos))
            pDrawing->DrawLine(entPos.x, entPos.y, pDrawing->screen_w / 2, pDrawing->screen_h, thick, color);

    }

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

    virtual const model_t* LoadModel(const char* pName, bool bProp = false) = 0;

    // Get accurate, sub-frame clock ( profiling use )
    virtual float				Time(void) = 0;

    // Get the exact server timesstamp ( server time ) from the last message received from the server
    virtual float				GetLastTimeStamp(void) = 0;

    // Given a CAudioSource (opaque pointer), retrieve the underlying CSentence object ( stores the words, phonemes, and close
    //  captioning data )
    virtual CSentence* GetSentence(CAudioSource* pAudioSource) = 0;
    // Given a CAudioSource, determines the length of the underlying audio file (.wav, .mp3, etc.)
    virtual float				GetSentenceLength(CAudioSource* pAudioSource) = 0;
    // Returns true if the sound is streaming off of the hard disk (instead of being memory resident)
    virtual bool				IsStreaming(CAudioSource* pAudioSource) const = 0;

    // Copy current view orientation into va
    virtual void				GetViewAngles(QAngle& va) = 0;
    // Set current view orientation from va
    virtual void				SetViewAngles(QAngle& va) = 0;

    // Retrieve the current game's maxclients setting
    virtual int					GetMaxClients(void) = 0;

    // Given the string pBinding which may be bound to a key, 
    //  returns the string name of the key to which this string is bound. Returns NULL if no such binding exists
    virtual	const char* Key_LookupBinding(const char* pBinding) = 0;

    // Given the name of the key "mouse1", "e", "tab", etc., return the string it is bound to "+jump", "impulse 50", etc.
    virtual const char* Key_BindingForKey(ButtonCode_t code) = 0;

    // key trapping (for binding keys)
    virtual void				StartKeyTrapMode(void) = 0;
    virtual bool				CheckDoneKeyTrapping(ButtonCode_t& code) = 0;

    // Returns true if the player is fully connected and active in game (i.e, not still loading)
    virtual bool				IsInGame(void) = 0;

};

void* GetInterface(const char *dllName, const char* interfaceName);
bool SDK_initInterfaces(void);

