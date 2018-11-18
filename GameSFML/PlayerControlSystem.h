#pragma once
#include "InputComponent.h"
#include "VelocityComponent.h"
#include "TransformComponent.h"
#include "entt/entt.hpp"

class PlayerControlSystem
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt)
	{
		ECS.view<InputComponent, VelocityComponent, TransformComponent>().each([&](auto & entity, InputComponent& input, VelocityComponent& vel, TransformComponent& trans ) {
			trans.position += dt * vel.moveSpeed * input.dir;
			trans.rotation += dt * vel.rotSpeed * input.rot;
		});
	}
};