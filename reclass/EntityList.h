#pragma once
#include "Inconstructible.h"
#include "hazedumper/csgo.hpp"
#include "Entity.h"
#include "Utils/mem.h"

#define ENTLIST_ELEM_SIZE 0x10

class EntityList
{
	INCONSTRUCTIBLE(EntityList);

public:
	Entity* getEntity(int id) noexcept {
		Entity* entPtr = mem::read<Entity*>((LPVOID)(this + id * ENTLIST_ELEM_SIZE));
		return entPtr;
	}
};