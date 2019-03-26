#pragma once
#include "../Graphics.h"
#include "entt/entt.hpp"
class IDrawSystem
{
public:
	virtual void Draw(Graphics& gfx, entt::DefaultRegistry& ECS) const = 0;
};