#include "pch.h"
#include "dx9.h"

namespace dx9
{
	bool GetD3D9Device(void** pTable, size_t size)
	{
		if (!pTable) return false;

		IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

		if (!pD3D) return false;

		IDirect3DDevice9* pDummy = nullptr;

		D3DPRESENT_PARAMETERS d3dpp = {};
		d3dpp.Windowed = false;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = GetProcessWindow();

		HRESULT result = pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			d3dpp.hDeviceWindow,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&pDummy
		);
		if (result != S_OK)
		{
			d3dpp.Windowed = ~d3dpp.Windowed;
			result = pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				d3dpp.hDeviceWindow,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&pDummy
			);
			if (result != S_OK)
			{
				pD3D->Release();
				return false;
			}
		}

		memcpy_s(pTable, size, *(void***)pDummy, size);
		pDummy->Release();
		pD3D->Release();
		return true;
	}

	BOOL CALLBACK enumWind(HWND handle, LPARAM lp)
	{
		DWORD procID;
		HWND* window = reinterpret_cast<HWND*>(lp);
		GetWindowThreadProcessId(handle, &procID);

		if (GetCurrentProcessId() != procID)
			return true;

		if (GetConsoleWindow() == handle)
			return true;

		*window = handle;
		return false;
	}

	HWND dx9::GetProcessWindow()
	{
		static HWND window = NULL;

		if (window != NULL) return window;

		EnumWindows(enumWind, (LPARAM)&window);

		if (!window) return NULL;

		return window;
	}

	void DrawLine(Math::Vec2 v1, Math::Vec2 v2, int thickness, D3DCOLOR color, LPDIRECT3DDEVICE9 pDevice)
	{
		static LPD3DXLINE pLine = nullptr;
		if (pLine == nullptr)
			D3DXCreateLine(pDevice, &pLine);
		D3DXVECTOR2 line[2];
		line[0] = D3DXVECTOR2(v1.x, v1.y);
		line[1] = D3DXVECTOR2(v2.x, v2.y);
		pLine->SetWidth(thickness);
		pLine->Draw(line, 2, color);
	}

	void DrawESPBox(Math::Vec2 top, Math::Vec2 bottom, int thickness, D3DCOLOR color, LPDIRECT3DDEVICE9 pDevice)
	{
		float height = std::abs(top.y - bottom.y);

		Math::Vec2 tl, tr, bl, br;

		tl.x = top.x - height / 4;
		tr.x = top.x + height / 4;
		tl.y = tr.y = top.y;
		
		bl.x = bottom.x - height / 4;
		br.x = bottom.x + height / 4;
		bl.y = br.y = bottom.y;

		DrawLine(tl, tr, thickness, color, pDevice);
		DrawLine(tr, br, thickness, color, pDevice);
		DrawLine(br, bl, thickness, color, pDevice);
		DrawLine(bl, tl, thickness, color, pDevice);
	}
}