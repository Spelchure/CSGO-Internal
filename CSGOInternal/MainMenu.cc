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
#include "StrLang.hpp"


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
    this->hValveWindow = hValveWindow;

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
    // Set original window proc
    SetWindowLongPtr(hValveWindow, GWL_WNDPROC, (LONG)(LONG_PTR)oriWndProc);
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
//    ImGui::ShowDemoWindow(0);
    if(ImGui::BeginTabBar("TB1")) {
        if(ImGui::BeginTabItem("Aimbot")) {
            ImGui::Checkbox(Lang::StringActive, &pSettings->boolSettings[pSettings->bAimbot]);
            ImGui::SameLine();
            ImGui::Checkbox(Lang::StringFOVact, &pSettings->boolSettings[pSettings->bAimbot_FOV]);
            ImGui::SameLine();
            ImGui::Checkbox(Lang::StringAimAutoAttack, &pSettings->boolSettings[pSettings->bAimbot_AutoAttack]);

            if(pSettings->getBool(pSettings->bAimbot_FOV)) 
                ImGui::SliderFloat(Lang::StringFOV, &pSettings->floatSettings[pSettings->fAimFOV], 1.000f, 80.000f, "%.3f", 1.0f);
                
            ImGui::SliderFloat(Lang::StringSmooth, &pSettings->floatSettings[pSettings->fAimSmooth], 1.000f, 25.000f, "%.3f", 1.0f);
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("ESP"))
        {
            ImGui::Checkbox(Lang::sESP_active, &pSettings->boolSettings[pSettings->bESP]);
            ImGui::SameLine();
            ImGui::Checkbox(Lang::sESP_active_mate, &pSettings->boolSettings[pSettings->bESP_mate]);
            
            ImGui::Checkbox(Lang::sSnap_active, &pSettings->boolSettings[pSettings->bSnaplines]);
            ImGui::SameLine();
            ImGui::Checkbox(Lang::sSnap_active_Mate, &pSettings->boolSettings[pSettings->bSnaplines_mate]);

            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Crosshair"))
        {
            //TODO
            ImGui::Text("todo");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Misc"))
        {
            ImGui::Checkbox("Bunnyhop", &pSettings->boolSettings[pSettings->bBunnyhop]);
            ImGui::Checkbox("Antiflash", &pSettings->boolSettings[pSettings->bAntiflash]);

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }


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






