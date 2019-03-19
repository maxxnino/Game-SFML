#pragma once
#include "entt/entt.hpp"
class ISystem
{
public:
	virtual void Update(entt::DefaultRegistry& ECS, float dt) = 0;
};