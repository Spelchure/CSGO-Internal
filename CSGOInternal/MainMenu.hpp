/*****************************************************************//**
 * \file   MainMenu.hpp
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
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
#include <d3d9.h>

class MainMenu {
    LPDIRECT3DDEVICE9 pDevice;
public:
    bool InitMenu(LPDIRECT3DDEVICE9 pD3Ddev, HWND hValveWindow);
    void ReleaseMenu(void);
    void PrintMenu(void);

    MainMenu() : pDevice(nullptr) {}
    ~MainMenu() { if(nullptr != pDevice) ReleaseMenu(); }
};

