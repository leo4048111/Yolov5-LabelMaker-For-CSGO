#pragma once
#include "Utils/Math.h"

namespace dx9 
{
	using tEndScene = HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9);

	bool GetD3D9Device(void** pTable, size_t size);

	static HWND window = NULL;

	HWND GetProcessWindow();

	void DrawLine(Math::Vec2 v1, Math::Vec2 v2, int thickness, D3DCOLOR color, LPDIRECT3DDEVICE9 pDevice);

	void DrawESPBox(Math::Vec2 top, Math::Vec2 bottom, int thickness, D3DCOLOR color, LPDIRECT3DDEVICE9 pDevice);
}
