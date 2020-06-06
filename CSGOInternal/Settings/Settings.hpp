/*****************************************************************//**
 * \file   Settings.hpp
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
#include <string>

constexpr auto numberOfBoolSettings = 10;
constexpr auto numberOfIntSettings = 1;
constexpr auto numberOfFloatSettings = 2;

/* Default settings 
bMenuShowing = false;
bAimbot = false;
*/

class AppSettings {
    std::string *filePath;
public:
    /* Required in ImGui menu ... */
    bool* boolSettings;
    int* intSettings;
    float* floatSettings;

    enum ColorSelect {
        cESP,
        cESP_mate,
        cSnaplines,
        cSnaplines_mate
    };
   
    enum BoolSettings {
        bMenuShowing,
        bAimbot, // Is aimbot open
        bAimbot_FOV, // Fov enabled
        bAimbot_AutoAttack, // Aim and Shoot !
        bESP,
        bESP_mate,
        bSnaplines,
        bSnaplines_mate,
        bBunnyhop,
        bAntiflash
    };
    enum IntSettings {
        iAimbotKey 
    };
    enum FloatSettings {
        fAimSmooth,
        fAimFOV
    };
    //Set default settings 
    AppSettings() {
        boolSettings = new bool[numberOfBoolSettings];
        intSettings = new int[numberOfIntSettings];
        floatSettings = new float[numberOfFloatSettings];
        filePath = nullptr; // Allocated in  parseFile
    }

    ~AppSettings() {
        if (nullptr != boolSettings)
            delete[] boolSettings;
        if (nullptr != intSettings)
            delete[] intSettings;
        if (nullptr != floatSettings)
            delete[] floatSettings;
        if (nullptr != filePath)
            delete filePath;
    }
    void setDefault(void) {
        for (int i = 0; i < numberOfBoolSettings; i++)
            boolSettings[i] = false;

        intSettings[iAimbotKey] = (int)'C';

        floatSettings[fAimFOV] = 40.0f;
        floatSettings[fAimSmooth] = 2.000001f;

    }
    bool parseFile(const char* path);
    void saveSettings(void);

    void setBool(int ind, bool val)
    {
        if (ind < numberOfBoolSettings)
            boolSettings[ind] = val;
    }
    bool getBool(int ind)
    {
        if (ind < numberOfBoolSettings)
            return boolSettings[ind];
        return false;
    }
    void toggleBool(int ind)
    {
        if (ind < numberOfBoolSettings)
            boolSettings[ind] = !boolSettings[ind];
    }
    void setInt(int ind, int val) {
        if (ind < numberOfIntSettings)
            intSettings[ind] = val;
        
    }
    int getInt(int ind)
    {
        if (ind < numberOfIntSettings)
            return intSettings[ind]; 
        return 0;
    }
        
    void setFloat(int ind, float v)
    {
        if (ind < numberOfFloatSettings)
            floatSettings[ind] = v;
    }
    float getFloat(int ind)
    {
        if (ind < numberOfFloatSettings)
            return floatSettings[ind];
        return 0.f;
    }
};

