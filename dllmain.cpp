// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "dx9/dx9.h"
#include "Utils/ScreenCapture.h"
#include "Utils/Esp.h"
#include "minhook/include/MinHook.h"
#pragma comment(lib, "minhook/libMinHook-x86-v141-mtd.lib")

#include "Utils/Interface.h"
#include "Config.h"

void* pD3D9Device[119];

static dx9::tEndScene endScene;

static dx9::tEndScene oEndScene = nullptr;

static bool isHackUninstall = false;

float boxHeightModifier = 1.2;

int imgCnt = 0;

int timer = SCREENSHOT_DELAY + 1;


void MainLoop(LPDIRECT3DDEVICE9 lpD3D9Device)
{
    if (GetAsyncKeyState(VK_INSERT) & 0x01) {
        isHackUninstall = true;
    }

    if (GetAsyncKeyState(VK_HOME) & 0x01) {
        timer = 0;
        LOG("Saved a screenshot!");
        return;
    }

    if (GetAsyncKeyState(VK_NUMPAD8) & 0x01) {
        boxHeightModifier += 0.1;
        LOG(boxHeightModifier);
    }

    if (GetAsyncKeyState(VK_NUMPAD2) & 0x01) {
        boxHeightModifier -= 0.1;
    }

    timer++;
    if (timer == SCREENSHOT_DELAY) {
        ScreenCapture::CaptureAnImage(dx9::GetProcessWindow());
    }

    if (timer >= SCREENSHOT_DELAY) {
        Esp::run(lpD3D9Device);
    }
}

HRESULT APIENTRY hkEndScene(LPDIRECT3DDEVICE9 lpD3D9Device)
{
    MainLoop(lpD3D9Device);
    return oEndScene(lpD3D9Device);
}

DWORD WINAPI HackThread(LPVOID params)
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    HMODULE hMod = (HMODULE)params;

    if (!dx9::GetD3D9Device(pD3D9Device, sizeof(pD3D9Device))) {
        LOG("Unable to get D3D9 Device");
        goto bail;
    }
    LOG("Found EndScene Entry Point:");
    LOG(oEndScene);
    if (MH_Initialize() != MB_OK)
    {
        LOG("Unable to initialize minhook");
        goto bail;
    }
    LOG("Hooking endscene");
    endScene = (dx9::tEndScene)pD3D9Device[42];
    LOG("Hooked endScene at " + std::to_string((int)endScene));
    MH_CreateHook((LPVOID)endScene, (LPVOID)hkEndScene, (LPVOID*)&oEndScene);
    MH_EnableHook((LPVOID)endScene);

    //initialize interfaces(this part is borrowed from Osiris)
    interfaces = std::make_unique<const Interfaces>();

    while (!isHackUninstall) {
        Sleep(500);
    }
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();

bail:
    fclose(fp);
    FreeLibraryAndExitThread(hMod, NULL);
    return NULL;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        HANDLE hThread = CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)HackThread, hModule, NULL, nullptr);
        CloseHandle(hThread);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

