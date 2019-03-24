#pragma once
#include "System/ISystemECS.h"
#include "Graphics.h"
#include "Component/HealthComponent.h"
#include "Component/PhysicComponent.h"
#include "Component/GameplayTags.h"
class HealthSystem : public ISystemECS
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		ECS.view<HealthComponent>().each([&ECS](auto entity, HealthComponent &Health) {
			
		});
	}
};