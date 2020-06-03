/*****************************************************************//**
 * \file   Signatures.hpp
 * \brief  
 * 
 * 
 * \author ALPEREN
 * \date   June 2020
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

#include <cstddef>

namespace signatures {
	constexpr ::std::ptrdiff_t anim_overlays = 0x2980;
	constexpr ::std::ptrdiff_t clientstate_choked_commands = 0x4D28;
	constexpr ::std::ptrdiff_t clientstate_delta_ticks = 0x174;
	constexpr ::std::ptrdiff_t clientstate_last_outgoing_command = 0x4D24;
	constexpr ::std::ptrdiff_t clientstate_net_channel = 0x9C;
	constexpr ::std::ptrdiff_t convar_name_hash_table = 0x2F0F8;
	constexpr ::std::ptrdiff_t dwClientState = 0x589DD4;
	constexpr ::std::ptrdiff_t dwClientState_GetLocalPlayer = 0x180;
	constexpr ::std::ptrdiff_t dwClientState_IsHLTV = 0x4D40;
	constexpr ::std::ptrdiff_t dwClientState_Map = 0x28C;
	constexpr ::std::ptrdiff_t dwClientState_MapDirectory = 0x188;
	constexpr ::std::ptrdiff_t dwClientState_MaxPlayer = 0x388;
	constexpr ::std::ptrdiff_t dwClientState_PlayerInfo = 0x52B8;
	constexpr ::std::ptrdiff_t dwClientState_State = 0x108;
	constexpr ::std::ptrdiff_t dwClientState_ViewAngles = 0x4D88;
	constexpr ::std::ptrdiff_t dwEntityList = 0x4D4B134;
	constexpr ::std::ptrdiff_t dwForceAttack = 0x317C71C;
	constexpr ::std::ptrdiff_t dwForceAttack2 = 0x317C728;
	constexpr ::std::ptrdiff_t dwForceBackward = 0x317C6D4;
	constexpr ::std::ptrdiff_t dwForceForward = 0x317C740;
	constexpr ::std::ptrdiff_t dwForceJump = 0x51F4DB0;
	constexpr ::std::ptrdiff_t dwForceLeft = 0x317C6C8;
	constexpr ::std::ptrdiff_t dwForceRight = 0x317C6EC;
	constexpr ::std::ptrdiff_t dwGameDir = 0x6286F8;
	constexpr ::std::ptrdiff_t dwGameRulesProxy = 0x526809C;
	constexpr ::std::ptrdiff_t dwGetAllClasses = 0xD5CF8C;
	constexpr ::std::ptrdiff_t dwGlobalVars = 0x589AD8;
	constexpr ::std::ptrdiff_t dwGlowObjectManager = 0x5292F18;
	constexpr ::std::ptrdiff_t dwInput = 0x519C6C8;
	constexpr ::std::ptrdiff_t dwInterfaceLinkList = 0x8FFB74;
	constexpr ::std::ptrdiff_t dwLocalPlayer = 0xD36B94;
	constexpr ::std::ptrdiff_t dwMouseEnable = 0xD3C738;
	constexpr ::std::ptrdiff_t dwMouseEnablePtr = 0xD3C708;
	constexpr ::std::ptrdiff_t dwPlayerResource = 0x317AA6C;
	constexpr ::std::ptrdiff_t dwRadarBase = 0x517FE8C;
	constexpr ::std::ptrdiff_t dwSensitivity = 0xD3C5D4;
	constexpr ::std::ptrdiff_t dwSensitivityPtr = 0xD3C5A8;
	constexpr ::std::ptrdiff_t dwSetClanTag = 0x89F00;
	constexpr ::std::ptrdiff_t dwViewMatrix = 0x4D3CA64;
	constexpr ::std::ptrdiff_t dwWeaponTable = 0x519D18C;
	constexpr ::std::ptrdiff_t dwWeaponTableIndex = 0x325C;
	constexpr ::std::ptrdiff_t dwYawPtr = 0xD3C398;
	constexpr ::std::ptrdiff_t dwZoomSensitivityRatioPtr = 0xD415E0;
	constexpr ::std::ptrdiff_t dwbSendPackets = 0xD3EFA;
	constexpr ::std::ptrdiff_t dwppDirect3DDevice9 = 0xA7030;
	constexpr ::std::ptrdiff_t find_hud_element = 0x29CCFD40;
	constexpr ::std::ptrdiff_t force_update_spectator_glow = 0x39CB92;
	constexpr ::std::ptrdiff_t interface_engine_cvar = 0x3E9EC;
	constexpr ::std::ptrdiff_t is_c4_owner = 0x3A93A0;
	constexpr ::std::ptrdiff_t m_bDormant = 0xED;
	constexpr ::std::ptrdiff_t m_flSpawnTime = 0xA360;
	constexpr ::std::ptrdiff_t m_pStudioHdr = 0x294C;
	constexpr ::std::ptrdiff_t m_pitchClassPtr = 0x5180130;
	constexpr ::std::ptrdiff_t m_yawClassPtr = 0xD3C398;
	constexpr ::std::ptrdiff_t model_ambient_min = 0x58CE4C;
	constexpr ::std::ptrdiff_t set_abs_angles = 0x1D2B60;
	constexpr ::std::ptrdiff_t set_abs_origin = 0x1D29A0;
}