/*****************************************************************//**
 * \file   Drawing.cc
 * \brief  
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
#include "Drawing.hpp"

#pragma warning(disable : 4244)

#define ABS(x) ((x < 0) ? (-x) : (x)) // Absolute value of x

void 
D3D9Drawing::DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color) {
    D3DRECT rect = { x,y,x + w,y + h };
    pDev->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void 
D3D9Drawing::DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color) {
    if (nullptr == lineInst)
        D3DXCreateLine(pDev, &lineInst);


    D3DXVECTOR2 Line[2];
    Line[0] = D3DXVECTOR2(x1, y1);
    Line[1] = D3DXVECTOR2(x2, y2);
    lineInst->SetWidth(thickness);
    lineInst->Draw(Line, 2, color);
}

void 
D3D9Drawing::DrawLine(Vector2 src, Vector2 dst, int thickness, D3DCOLOR color) {
    this->DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

void 
D3D9Drawing::DrawEspBox(Vector2 top, Vector2 bot, int thickness, D3DCOLOR color) {
    int height = ABS(top.y - bot.y);

    Vector2 tl, tr;
    tl.x = top.x - height / 4;
    tr.x = top.x + height / 4;
    tl.y = tr.y = top.y;

    Vector2 bl, br;
    bl.x = bot.x - height / 4;
    br.x = bot.x + height / 4;
    bl.y = br.y = bot.y;

    DrawLine(tl, tr, thickness, color);
    DrawLine(bl, br, thickness, color);
    DrawLine(tl, bl, thickness, color);
    DrawLine(tr, br, thickness, color);
}

bool 
D3D9Drawing::World2Screen(float* viewMatrix, Vector3 pos, Vector2& screen)
{
    Vector4 clipCoords;
    clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] + pos.z * viewMatrix[2] + viewMatrix[3];
    clipCoords.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] + pos.z * viewMatrix[6] + viewMatrix[7];
    clipCoords.z = pos.x * viewMatrix[8] + pos.y * viewMatrix[9] + pos.z * viewMatrix[10] + viewMatrix[11];
    clipCoords.w = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] + pos.z * viewMatrix[14] + viewMatrix[15];

    if (clipCoords.w < 0.1f)
        return false;

    Vector NDC;
    NDC.x = clipCoords.x / clipCoords.w;
    NDC.y = clipCoords.y / clipCoords.w;
    NDC.z = clipCoords.z / clipCoords.w;

    screen.x = (screen_w / 2 * NDC.x) + (NDC.x + screen_w / 2);
    screen.y = -(screen_h / 2 * NDC.y) + (NDC.y + screen_h / 2);
    return true;

}
