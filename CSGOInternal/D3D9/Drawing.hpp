/*****************************************************************//**
 * \file   Drawing.hpp
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
#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "../Source SDK/Vectors.hpp"


class D3D9Drawing {
    LPDIRECT3DDEVICE9 pDev;
    ID3DXLine* lineInst;
    //ID3DXFont* FontF;
public:
    int screen_h, screen_w;

    D3D9Drawing(LPDIRECT3DDEVICE9 pDevice) : pDev(pDevice), 
        screen_h(0), screen_w(0), lineInst(nullptr) {}
    
    ~D3D9Drawing() {
        if (nullptr != lineInst)
            lineInst->Release();
    }
    
    void SetDevice(LPDIRECT3DDEVICE9 pDevice) {
        pDev = pDevice;
    }
    void GetReady() {
        
        /* Get viewport (screen size ) */
        D3DVIEWPORT9 viewPort;
        pDev->GetViewport(&viewPort);
        screen_w = viewPort.Width; // +viewPort.X;
        screen_h = viewPort.Height; // + viewPort.Y;

        D3DXCreateLine(pDev, &lineInst);


    }

    void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color);
    void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);
    void DrawLine(Vector2 src, Vector2 dst, int thickness, D3DCOLOR color);
    void DrawEspBox(Vector2 top, Vector2 bot, int thickness, D3DCOLOR color);
    bool World2Screen(float* viewMatrix, Vector3 pos, Vector2& screen);
};



