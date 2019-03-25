#pragma once
#include "Locator.h"
#include "System/ISystemECS.h"

class PhysicSystem : public ISystemECS
{
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		if (Locator::Physic::empty()) return;
		Locator::Physic::ref().Step(dt, 4, 2);
	}
};