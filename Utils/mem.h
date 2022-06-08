#pragma once
#include <Windows.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
#include <limits>
#include <string_view>
#include <utility>
#include <Psapi.h>

#include "reclass/Platform.h"

namespace mem
{
	template<typename T>
	void patch(LPVOID dst, T src);

	template<typename T>
	T read(LPVOID src);

    template <typename T>
    static constexpr auto relativeToAbsolute(uintptr_t address) noexcept
    {
        return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
    }

    struct ModuleInfo {
        void* base;
        std::size_t size;
    };

    static ModuleInfo getModuleInformation(const char* name) noexcept
    {
        if (HMODULE handle = GetModuleHandleA(name)) {
            if (MODULEINFO moduleInfo; GetModuleInformation(GetCurrentProcess(), handle, &moduleInfo, sizeof(moduleInfo)))
                return ModuleInfo{ moduleInfo.lpBaseOfDll, moduleInfo.SizeOfImage };
        }
        return {};
    }

    [[nodiscard]] static auto generateBadCharTable(std::string_view pattern) noexcept
    {
        assert(!pattern.empty());

        std::array<std::size_t, (std::numeric_limits<std::uint8_t>::max)() + 1> table;

        auto lastWildcard = pattern.rfind('?');
        if (lastWildcard == std::string_view::npos)
            lastWildcard = 0;

        const auto defaultShift = (std::max)(std::size_t(1), pattern.length() - 1 - lastWildcard);
        table.fill(defaultShift);

        for (auto i = lastWildcard; i < pattern.length() - 1; ++i)
            table[static_cast<std::uint8_t>(pattern[i])] = pattern.length() - 1 - i;

        return table;
    }

    template <bool ReportNotFound = true>
    static std::uintptr_t findPattern(ModuleInfo moduleInfo, std::string_view pattern) noexcept
    {
        static auto id = 0;
        ++id;

        if (moduleInfo.base && moduleInfo.size) {
            const auto lastIdx = pattern.length() - 1;
            const auto badCharTable = generateBadCharTable(pattern);

            auto start = static_cast<const char*>(moduleInfo.base);
            const auto end = start + moduleInfo.size - pattern.length();

            while (start <= end) {
                int i = lastIdx;
                while (i >= 0 && (pattern[i] == '?' || start[i] == pattern[i]))
                    --i;

                if (i < 0)
                    return reinterpret_cast<std::uintptr_t>(start);

                start += badCharTable[static_cast<std::uint8_t>(start[lastIdx])];
            }
        }

        assert(false);
        return 0;
    }

    template <bool ReportNotFound = true>
    static std::uintptr_t findPattern(const char* moduleName, std::string_view pattern) noexcept
    {
        return findPattern<ReportNotFound>(getModuleInformation(moduleName), pattern);
    }

    static uintptr_t jmpEbxGadgetInClient = findPattern(CLIENT_DLL, "\x1B\xFF\x23\xF8\xF6\x87") + 1;
};

template<typename T>
T mem::read(LPVOID src)
{
	T buffer;
	size_t size = sizeof(T);
	memcpy_s(&buffer, size, src, size);
	return buffer;
}

template<typename T>
void mem::patch(LPVOID dst, T src)
{
	DWORD oldProtect;
	size_t size = sizeof(src);
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy_s(dst, size, &src, size);
	VirtualProtect(dst, size, oldProtect, &oldProtect);
}