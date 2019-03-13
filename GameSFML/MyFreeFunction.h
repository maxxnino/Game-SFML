#pragma once
#include "Locator.h"
#include "PhysicComponent.h"
#include "AnimationComponent.h"
namespace Maxx
{
	/*void DestroyPhysicComponent(entt::DefaultRegistry& ECS, uint32_t entity)
	{
		auto& physic = ECS.get<PhysicComponent>(entity);
		Locator::Physic::ref().DestroyBody(physic.body);
	}*/
	void ContructComponent(entt::DefaultRegistry&, uint32_t entity)
	{

	}
}