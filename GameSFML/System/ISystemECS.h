#pragma once
#include "entt/entt.hpp"
#include <execution>
class ISystemECS
{
public:
	virtual void Update(entt::DefaultRegistry& ECS, float dt) = 0;
};