#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <Windows.h>
#include "reclass/Platform.h"

#define GAME_INTERFACE(type, name, moduleName, version) \
type* name = reinterpret_cast<type*>(find(moduleName, version));

class EngineTrace;

class Interfaces 
{
public:
    GAME_INTERFACE(EngineTrace, engineTrace, ENGINE_DLL, "EngineTraceClient004")

private:
    static void* find(const char* moduleName, const char* name) noexcept
    {
        if (const auto createInterface = reinterpret_cast<std::add_pointer_t<void* CDECL_CONV(const char* name, int* returnCode)>>(
            GetProcAddress(GetModuleHandleA(moduleName), "CreateInterface")

            )) {
            if (void* foundInterface = createInterface(name, nullptr))
            {
                return foundInterface;
            }
        }
        LOG("Error");
        std::exit(EXIT_FAILURE);
    }
};

inline std::unique_ptr<const Interfaces> interfaces;
