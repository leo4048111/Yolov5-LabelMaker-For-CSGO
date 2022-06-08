#pragma once

#include <cstddef>

#include "Platform.h"

#ifdef _WIN32
#include "x86RetSpoof.h"
#include "Utils/mem.h"
#endif

namespace VirtualMethod
{
    template <typename T, std::size_t Idx, typename... Args>
    constexpr T call(void* classBase, Args... args) noexcept
    {
        return x86RetSpoof::invokeThiscall<T, Args...>(std::uintptr_t(classBase), (*reinterpret_cast<std::uintptr_t**>(classBase))[Idx], mem::jmpEbxGadgetInClient, args...);
    }
}

#define VIRTUAL_METHOD(returnType, name, idx, args, argsRaw) \
returnType name args noexcept \
{ \
    return VirtualMethod::call<returnType, idx>argsRaw; \
}

#define VIRTUAL_METHOD_V(returnType, name, idx, args, argsRaw) VIRTUAL_METHOD(returnType, name, idx, args, argsRaw)

