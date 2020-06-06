/*****************************************************************//**
 * \file   StrLang.hpp
 * \brief  Language specific strings
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

#define LANG_EN_US
//#define LANG_TR_TR


namespace Lang {
#ifdef LANG_EN_US
    constexpr auto StringActive = "Active";
    constexpr auto StringFOVact = "FOV active";
    constexpr auto StringFOV = "FOV";
    constexpr auto StringSmooth = "Aim smooth";
    constexpr auto StringAimAutoAttack = "Aim auto attack";

    /** ESP */
    constexpr auto sESP_color_mate = "ESP (mate) color";
    constexpr auto sESP_color = "ESP (enemy) color";
    constexpr auto sESP_active = "ESP (enemy) active ";
    constexpr auto sESP_active_mate = "ESP (mate) active ";

    /**  Snaplines */
    constexpr auto sSnap_active = "Snaplines (enemy) active";
    constexpr auto sSnap_active_Mate = "Snaplines (mate) active";
    constexpr auto sSnap_color = "Snaplines (enemy) color";
    constexpr auto sSnap_color_mate = "Snaplines (mate) color";

#elif defined(LANG_TR_TR)
    constexpr auto StringActive = "Aktif";
    constexpr auto StringFOVact = "FOV Aktif";
    constexpr auto StringFOV = "FOV";
    constexpr auto StringSmooth = "Aim hýzý";
    constexpr auto StringAimAutoAttack = "Aim otomatik atýþ";
    constexpr auto sESP_color_mate = "ESP (takým) renk";
#endif
}
