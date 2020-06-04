/*****************************************************************//**
 * \file   Strings.hpp
 * \brief  
 * 
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

/* Constants for SDK */

constexpr auto VALVE_WINDOW_CLASS = "Valve001";

constexpr auto DLL_CLIENT = "client.dll"; // new game update
constexpr auto DLL_ENGINE = "engine.dll";
constexpr auto DLL_SHADERAPI = "shaderapidx9.dll";

constexpr auto VCLIENTENTITYLIST_INTERFACE_VERSION = "VClientEntityList003"; 
constexpr auto CLIENT_DLL_INTERFACE_VERSION = "VClient018";
constexpr auto VENGINE_CLIENT_INTERFACE_VERSION = "VEngineClient014";
constexpr auto VENGINE_CLIENT_INTERFACE_VERSION_13 = "VEngineClient013";
constexpr auto ENGINE_TRACE_CLIENT_VERSION = "EngineTraceClient004";


constexpr const char *PATTERN_CLIENTSTATE[] = {
    "\xA1\x90\x90\x90\x90\x33\xD2\x6A\x00\x6A\x00\x33\xC9\x89\xB0",
    "x????xxxxxxxxxx"
};
constexpr const char* PATTERN_CLIENTSTATE_MAXPLAYERS[] = {
    "\xA1\x90\x90\x90\x90\x8B\x80\x90\x90\x90\x90\xC3\xCC\xCC\xCC\xCC\x55\x8B\xEC\x8A\x45\x08",
    "x????xx????xxxxxxxxxxx",
};
constexpr const char* PATTERN_CLIENTSTATE_VIEWANGLES[] = {
    "\xF3\x0F\x11\x80\x90\x90\x90\x90\xD9\x46\x04\xD9\x05",
    "xxxx????xxxxx"
};
constexpr const char* PATTERN_BDORMANT[] = {
    "\x8A\x81\x90\x90\x90\x90\xC3\x32\xC0",
    "xx????xxx"
};
constexpr const char* PATTERN_PTRD3DDEV[] = {
    "\xA1\x90\x90\x90\x90\x50\x8B\x08\xFF\x51\x0C",
    "x????xxxxxx"
};