#pragma once
#include "Box2D/Box2D.h"
#include "entt/entt.hpp"
#include <queue>
class Box2DContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact) final
	{
		auto b1 = contact->GetFixtureA()->GetBody();
		auto b2 = contact->GetFixtureB()->GetBody();
		if (b1->GetType() == b2_dynamicBody && b2->GetType() == b2_dynamicBody)
		{
			pairCollied.emplace(std::make_pair(b1->GetUserEntity(), b2->GetUserEntity()));
		}
	}
	std::queue<std::pair<uint32_t, uint32_t>> pairCollied;
};