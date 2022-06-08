#include "pch.h"
#include "Esp.h"
#include "Config.h"
#include "hazedumper/csgo.hpp"
#include "mem.h"
#include "dx9/dx9.h"
#include "reclass/Entity.h"
#include "reclass/EntityList.h"

namespace Esp
{
    void run(LPDIRECT3DDEVICE9 pDevice)
    {
        static uintptr_t engineBase = (uintptr_t)GetModuleHandle("engine.dll");
        static uintptr_t clientBase = (uintptr_t)GetModuleHandle("client.dll");
        uintptr_t clientState = mem::read<uintptr_t>((LPVOID)(engineBase + hazedumper::signatures::dwClientState));
        uint32_t localPlayerID = mem::read<uint32_t>((LPVOID)(clientState + hazedumper::signatures::dwClientState_GetLocalPlayer));
        uint32_t mask = (1 << localPlayerID);
        Entity* localPlayer = mem::read<Entity*>((LPVOID)(clientBase + hazedumper::signatures::dwLocalPlayer));
        EntityList* entList = (EntityList*)(clientBase + hazedumper::signatures::dwEntityList);
        float viewMatrix[16];

        if (localPlayer == NULL) goto done;

        memcpy_s(&viewMatrix, sizeof(viewMatrix), (PBYTE*)(clientBase + hazedumper::signatures::dwViewMatrix), sizeof(viewMatrix));

        for (uint32_t i = 0; i < 64; i++)
        {
            Entity* player = entList->getEntity(i);
            if (player == NULL) continue;

            if (player->getHealth() < 1) continue;

            /*uint32_t bSpottedByMask = player->getBSpottedByMask();
            if (!(bSpottedByMask & mask)) continue;*/

            if (!player->isVisible(localPlayer, {NULL, NULL, NULL})) continue;

            Math::Vec3 headPos3D = player->getBonePos(8);
            Math::Vec3 feetPos3D = player->getVecOrigin();

            Math::Vec2 headPos2D, feetPos2D;
            if (!(Math::WorldToScreen(viewMatrix, headPos3D, headPos2D) && Math::WorldToScreen(viewMatrix, feetPos3D, feetPos2D)))
            {
                LOG("Error conversion");
                continue;
            }

            dx9::DrawESPBox(headPos2D, feetPos2D, 2, D3DCOLOR_RGBA(255, 0, 0, 255), pDevice);
        }

    done:
        return;
    }
}
