/*****************************************************************//**
 * \file   DummyDevice.cc
 * \brief  
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
#include "DummyDevice.hpp"
#include <d3d9.h>



/**
 * \brief Gets d3d9device pointer using dummy device method
 * 
 * \param[in] hWind Handle to focus window required in d3d present parameters
 * \param[out] pTable Pointer to table -> void *vtable[119];
 * \param[in] size size in bytes
 * \returns True on success
 */
bool 
GetD3D9Device(HWND hWind, void** pTable, size_t size)
{
    if (!pTable)
        return false;

    IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION); // Create context

    if (!pD3D)
        return false;

    IDirect3DDevice9* pDummyDevice = NULL; // Our dummy device

    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.Windowed = false;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // Discard swap effect not required
    d3dpp.hDeviceWindow = hWind;

    HRESULT hOk =
        pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

    if(S_OK != hOk) { // Can be fail in windowed or fullscreen mode
        d3dpp.Windowed = !d3dpp.Windowed;
        hOk =
            pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow,
                D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);
        if (S_OK != hOk) { // Cannot create dummy device
            pD3D->Release();
            return false;
        }
    }

    memcpy(pTable, *reinterpret_cast<void***>(pDummyDevice), size);
    pDummyDevice->Release();
    pD3D->Release();
    return true;
}

