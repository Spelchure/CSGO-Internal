/*****************************************************************//**
 * \file   Settings.cc
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
#include "Settings.hpp"
#include <iostream>
#include <fstream>
#include <regex>

bool 
AppSettings::parseFile(const char* path)
{
    if (path == nullptr)
        return false;
    std::ifstream fStream;
    fStream.open(path, std::ios_base::in);
    
    if (fStream.fail())
        return false;
    
    //Read settings file.
    std::string line;

    const char* regex_[] = {
        "([0-9]+)\\s*=\\s*([01]);" // Match boolean
    };
    int ind = 0;
    while (std::getline(fStream, line))
    {
        if(!line.compare("[bool]")) { // Boolean settings
            ind = 0;
            continue;
        } else if(!line.compare("[float]")) {
            ind = 1;
            continue;
        }
    
        std::regex rg(regex_[ind]); // Regex
        std::smatch match;
        if(regex_search(line,match,rg)) {
            if (match.size() > 2) {
                if (ind == 0) // Boolean
                {
                    int val = std::stoi(match[2].str());
                    this->setBool(std::stoi(match[1].str()), val != 0);
                }
                else if (ind == 1) { // Float 
                
                }
            }
        } else {  // Invalid file format
            fStream.close();
            return false;
        }
    }

    this->filePath = new std::string(path);

    fStream.close();
    return true;
}

void 
AppSettings::saveSettings(void)
{
    if (nullptr == filePath)
        return;

    std::ofstream fStream (filePath->c_str(), std::ios_base::out | std::ios_base::trunc);

    if (fStream.fail())
        return;

    // Write Boolean settings
    fStream << "[bool]\n";
    for (int i = 0; i < numberOfBoolSettings; i++) // Boolean settings
    {
        fStream << std::to_string(i) << " = " << (boolSettings[i] ? "1;" : "0;") << std::endl;
    }

    //Write others
    fStream.close();
    
}
