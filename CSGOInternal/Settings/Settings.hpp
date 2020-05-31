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

constexpr auto numberOfBoolSettings = 1;
constexpr auto numberOfFloatSettings = 0;
constexpr auto numberOfIntSettings = 1;

/* Default settings 
Menu Showing = false;

*/

class AppSettings {
    bool* boolSettings;
    int* intSettings;
    
    std::string *filePath;
public:
    enum BoolSettings {
        bMenuShowing,
    };
    enum IntSettings {
        iAimbotKey 
    };
    //Set default settings 
    AppSettings() {
        boolSettings = new bool[numberOfBoolSettings];
        intSettings = new int[numberOfIntSettings];

        filePath = nullptr; // Allocated in  parseFile
    }

    ~AppSettings() {
        if (nullptr != boolSettings)
            delete[] boolSettings;
        if (nullptr != filePath)
            delete filePath;
    }
    void setDefault(void) {
        boolSettings[bMenuShowing] = false;
        intSettings[iAimbotKey] = (int)'C';
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
};

