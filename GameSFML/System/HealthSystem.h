#pragma once
#include "../System/ISystemECS.h"
#include "../Graphics.h"
#include "../Component/HealthComponent.h"
#include "../Component/PhysicComponent.h"
#include "../Component/GameplayTags.h"
class HealthSystem : public ISystemECS
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		ECS.view<HealthComponent>().each([&ECS](auto entity, HealthComponent &Health) {
			if (Health.curHealth > Health.maxHealth)
			{
				Health.curHealth = Health.maxHealth;
				return;
			}
			if (Health.curHealth > 0) return;

			ECS.assign<DeathTag>(entity);
		});
	}
};