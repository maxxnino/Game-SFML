#pragma once
#include "Box2D/Box2D.h"
struct CollisionCallbackComponent
{
	uint16 selfCatagory;
	// First: other entity - Second: categotyMask
	std::vector<std::pair<uint32_t, uint16>> others;
};