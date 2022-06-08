#include "pch.h"
#include "LabelMaker.h"
#include "Config.h"
#include "hazedumper/csgo.hpp"
#include "mem.h"
#include "reclass/Entity.h"
#include "reclass/EntityList.h"

namespace LabelMaker
{
    int makeLabel() {
        static uintptr_t engineBase = (uintptr_t)GetModuleHandle("engine.dll");
        static uintptr_t clientBase = (uintptr_t)GetModuleHandle("client.dll");
        uintptr_t clientState = mem::read<uintptr_t>((LPVOID)(engineBase + hazedumper::signatures::dwClientState));
        uint32_t localPlayerID = mem::read<uint32_t>((LPVOID)(clientState + hazedumper::signatures::dwClientState_GetLocalPlayer));
        LOG(localPlayerID);
        uint32_t mask = (1 << localPlayerID);
        Entity* localPlayer = mem::read<Entity*>((LPVOID)(clientBase + hazedumper::signatures::dwLocalPlayer));
        EntityList* entList = (EntityList*)(clientBase + hazedumper::signatures::dwEntityList);
        if (localPlayer == NULL) goto done;

        for (uint32_t i = 0; i < 64; i++)
        {
            Entity* player = entList->getEntity(i);
            if (player == NULL) continue;

            uint32_t bSpottedByMask = player->getBSpottedByMask();
            if (!(bSpottedByMask & mask)) continue;

            LOG(player->getHealth());
        }




    done:
        return 0;
    }
}
