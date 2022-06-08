#include "pch.h"
#include "Math.h"
#include "dx9/dx9.h"

namespace Math
{
	bool WorldToScreen(float* viewMatrix, Vec3 pos, Vec2& screenCoord) {
		Vec4 clipCoords;
		clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] + pos.z * viewMatrix[2] + viewMatrix[3];
		clipCoords.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] + pos.z * viewMatrix[6] + viewMatrix[7];
		clipCoords.z = pos.x * viewMatrix[8] + pos.y * viewMatrix[9] + pos.z * viewMatrix[10] + viewMatrix[11];
		clipCoords.w = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] + pos.z * viewMatrix[14] + viewMatrix[15];

		if (clipCoords.w < 0.1f) return false;

		Vec3 NDC;
		NDC.x = clipCoords.x / clipCoords.w;
		NDC.y = clipCoords.y / clipCoords.w;
		NDC.z = clipCoords.z / clipCoords.w;

		RECT size;
		GetWindowRect(dx9::GetProcessWindow(), &size);
		LONG windowWidth = size.right - size.left;
		LONG windowHeight = size.bottom - size.top;

		windowHeight -= 29;
		windowWidth -= 5;

		screenCoord.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
		screenCoord.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);

		return true;
	}
}