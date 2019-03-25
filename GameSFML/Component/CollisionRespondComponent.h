#pragma once
#include "Box2D/Box2D.h"
#include "entt/entt.hpp"
struct CollisionCallbackData
{
	// First: other entity - Second: categotyMask
	std::vector<std::pair<uint32_t, uint16>> others;
};
struct CollisionRespondComponent
{
	//this will use by CollisionRespondSystem, and this system will use multi-thread don't remove or add new Component
	entt::Delegate<void(uint32_t, entt::DefaultRegistry&)> myDelegate;
};