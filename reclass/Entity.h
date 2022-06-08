#pragma once
#include "Inconstructible.h"
#include "hazedumper/csgo.hpp"
#include "Utils/Math.h"
#include "Utils/Interface.h"
#include "reclass/EngineTrace.h"


class Entity
{
	INCONSTRUCTIBLE(Entity);

public:
	bool getBDormant() noexcept {
		return *reinterpret_cast<bool*>(this + hazedumper::signatures::m_bDormant);
	}

	int getHealth() noexcept {
		return *reinterpret_cast<int*>(this + hazedumper::netvars::m_iHealth);
	}

	uint32_t getBSpottedByMask() noexcept {
		return *reinterpret_cast<uint32_t*>(this + hazedumper::netvars::m_bSpottedByMask);
	}

	Math::Vec3 getBonePos(int boneId) noexcept {
		Math::Vec3 bonePos;
		uintptr_t boneBase = mem::read<uintptr_t>((LPVOID)(this + hazedumper::netvars::m_dwBoneMatrix));
		bonePos.x = mem::read<float>((LPVOID)(boneBase + 0x30 * boneId + 0x0C));
		bonePos.y = mem::read<float>((LPVOID)(boneBase + 0x30 * boneId + 0x1C));
		bonePos.z = mem::read<float>((LPVOID)(boneBase + 0x30 * boneId + 0x2C));
		return bonePos;
	}

	Math::Vec3 getVecOrigin() noexcept {
		return *reinterpret_cast<Math::Vec3*>(this + hazedumper::netvars::m_vecOrigin);
	}

	auto getEyePosition() noexcept {
		Vector v;
		VirtualMethod::call<void, 285>(this, std::ref(v));
		return v;
	}

	uint32_t getTeamID() noexcept {
		return *reinterpret_cast<uint32_t*>(this + hazedumper::netvars::m_iTeamNum);
	}

	bool isVisible(Entity* localPlayer, const Math::Vector& position = {0, 0, 0}) noexcept {
		if (!localPlayer)
			return false;

		Trace trace;
		interfaces->engineTrace->traceRay({ localPlayer->getEyePosition(), this->getBonePos(8)}, 0x46004009, {localPlayer}, trace);
		return trace.entity == this || trace.fraction > 0.87f;
	}
};

