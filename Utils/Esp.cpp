#include "pch.h"
#include <vector>
#include "Esp.h"
#include "Config.h"
#include "hazedumper/csgo.hpp"
#include "mem.h"
#include "dx9/dx9.h"
#include "reclass/Entity.h"
#include "reclass/EntityList.h"
#include "Utils/LabelMaker.h"

extern float boxHeightModifier;

namespace Esp
{
    void run(LPDIRECT3DDEVICE9 pDevice)
    {
        static uintptr_t engineBase = (uintptr_t)GetModuleHandle("engine.dll");
        static uintptr_t clientBase = (uintptr_t)GetModuleHandle("client.dll");
        uintptr_t clientState = mem::read<uintptr_t>((LPVOID)(engineBase + hazedumper::signatures::dwClientState));
        uint32_t localPlayerID = mem::read<uint32_t>((LPVOID)(clientState + hazedumper::signatures::dwClientState_GetLocalPlayer));
        Entity* localPlayer = mem::read<Entity*>((LPVOID)(clientBase + hazedumper::signatures::dwLocalPlayer));
        EntityList* entList = (EntityList*)(clientBase + hazedumper::signatures::dwEntityList);
        float viewMatrix[16];
        std::vector<Math::Vec5> labels;

        if (localPlayer == NULL) goto done;

        memcpy_s(&viewMatrix, sizeof(viewMatrix), (PBYTE*)(clientBase + hazedumper::signatures::dwViewMatrix), sizeof(viewMatrix));

        for (uint32_t i = 0; i < 64; i++)
        {
            if (i == localPlayerID) continue;
            Entity* player = entList->getEntity(i);
            if (player == NULL) continue;

            if (player->getHealth() < 1) continue;

            if (!player->isVisible(localPlayer, {NULL, NULL, NULL})) continue;

            Math::Vec3 headPos3D = player->getBonePos(8);
            Math::Vec3 feetPos3D = player->getVecOrigin();

            Math::Vec2 headPos2D, feetPos2D;
            if (!(Math::WorldToScreen(viewMatrix, headPos3D, headPos2D) && Math::WorldToScreen(viewMatrix, feetPos3D, feetPos2D)))
            {
                continue;
            }

            Math::Vec2 midPoint, boxSize, headSize;
            midPoint.x = (headPos2D.x + feetPos2D.x) / 2;
            midPoint.y = headPos2D.y + (feetPos2D.y - headPos2D.y) / 2;
            boxSize.x = (feetPos2D.y - headPos2D.y) / 2;
            boxSize.y = (feetPos2D.y - headPos2D.y) * boxHeightModifier;
            headSize.x = boxSize.x * 3 / 8;
            headSize.y = boxSize.y * 8 / 63;

            //dx9::DrawESPBox(headPos2D, feetPos2D, 2, D3DCOLOR_RGBA(255, 0, 0, 255), pDevice);
            dx9::DrawESPBox2(midPoint, boxSize, 2, D3DCOLOR_RGBA(255, 0, 0, 255), pDevice);
            dx9::DrawESPBox2(headPos2D, headSize, 1, D3DCOLOR_RGBA(255, 255, 0, 255), pDevice);


            if (timer == SCREENSHOT_DELAY) {
                RECT size;
                GetWindowRect(dx9::GetProcessWindow(), &size);
                LONG windowWidth = size.right - size.left;
                LONG windowHeight = size.bottom - size.top;

                windowHeight -= 29;
                windowWidth -= 5;

                uint32_t teamID = player->getTeamID();
                if (teamID == 2)     //2 for terrorist
                {
                    labels.push_back({ (int)LabelMaker::Labels::T, midPoint.x / windowWidth, midPoint.y / windowHeight, boxSize.x / windowWidth, boxSize.y / windowHeight });
                    labels.push_back({ (int)LabelMaker::Labels::T_HEAD, headPos2D.x / windowWidth, headPos2D.y / windowHeight, headSize.x / windowWidth, headSize.y / windowHeight });
                }
                else if (teamID == 3)  //3 for counter terrorist
                {
                    labels.push_back({ (int)LabelMaker::Labels::CT, midPoint.x / windowWidth, midPoint.y / windowHeight, boxSize.x / windowWidth, boxSize.y / windowHeight });
                    labels.push_back({ (int)LabelMaker::Labels::CT_HEAD, headPos2D.x / windowWidth, headPos2D.y / windowHeight, headSize.x / windowWidth, headSize.y / windowHeight });
                }
            }
        }

        if (timer == SCREENSHOT_DELAY) {
            LabelMaker::makeLabel(labels);
            imgCnt++;
        }

    done:
        return;
    }
}
