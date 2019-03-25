#pragma once
#include "Box2D/Box2D.h"
#include "entt/entt.hpp"
struct CollisionCallbackData
{
	uint16 selfCatagory;
	// First: other entity - Second: categotyMask
	std::vector<std::pair<uint32_t, uint16>> others;
};
struct CollisionRespondComponent
{
	entt::SigH<void(uint32_t, entt::DefaultRegistry&)> signal;
};