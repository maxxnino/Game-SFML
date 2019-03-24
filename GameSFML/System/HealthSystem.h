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
			if (Health.curHealth <= 0)
			{
				ECS.assign<DeathTag>(entity);
				return;
			}

			if (Health.curHealth >= 100)
			{
				ECS.assign<DeathTag>(entity);
				ECS.assign<SpawnEnemyInfo>(entity);
			}
		});
	}
};