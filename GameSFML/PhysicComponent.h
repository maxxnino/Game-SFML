#pragma once
#include "Box2D/Box2D.h"
#include "CollisionFilter.h"
#include <memory>
#include <functional>

struct PhysicComponent
{
	std::unique_ptr<b2Body, std::function<void(b2Body*)>> body;
};