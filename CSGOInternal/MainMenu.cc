/*****************************************************************//**
 * \file   MainMenu.cc
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
#include "MainMenu.hpp"
#include "Settings/Settings.hpp"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
WNDPROC oriWndProc = 0x0;

extern  IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern AppSettings* pSettings;

/**
 * \brief Init ImGui menu
 * 
 * \return 
 */
bool 
MainMenu::InitMenu(LPDIRECT3DDEVICE9 pD3Ddev, HWND hValveWindow)
{
    if (pD3Ddev == nullptr)
        return false;
    this->pDevice = pD3Ddev;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();
    if (!ImGui_ImplWin32_Init(hValveWindow))
        return false;
    if (!ImGui_ImplDX9_Init(pDevice))
        return false;

    // Hook wndproc
    oriWndProc = (WNDPROC)SetWindowLongPtr(hValveWindow, GWL_WNDPROC, (LONG)(LONG_PTR)WndProc);
    return true;
}

void 
MainMenu::ReleaseMenu()
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

/**
 * \brief Prints menu to screen
 * 
 */
void 
MainMenu::PrintMenu()
{
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags flags = ImGuiWindowFlags_None;
    flags |= ImGuiWindowFlags_NoSavedSettings;
    flags |= ImGuiWindowFlags_NoTitleBar;
   
    ImGui::Begin("CSGO", 0,flags);
    ImGui::Text("Testing..");
    ImGui::End();

    // Render
    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

    
}


/**
 * \brief Hooked window proc 
 */
LRESULT 
CALLBACK
WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if(pSettings->getBool(pSettings->bMenuShowing)) {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
            return 1L;
    }
    return CallWindowProc(oriWndProc, hwnd, msg, wParam, lParam); // Call default window proc
}






