#pragma once
#include "PhysicComponent.h"

class PhysicSystem
{
public:
	void Update(b2World& box2DEngine, float dt)
	{
		box2DEngine.Step(dt, 4, 2);
	}
};