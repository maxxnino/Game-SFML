#pragma once
#include "Box2D/Box2D.h"
#include <memory>
#include <functional>

enum CollisionFillter {
	BORDER = 0x0001,
	ENEMY = 0x0002,
	PLAYER = 0x0004
};

struct PhysicComponent
{
	std::unique_ptr<b2Body, std::function<void(b2Body*)>> body;
};