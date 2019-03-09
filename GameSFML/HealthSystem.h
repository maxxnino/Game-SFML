#pragma once
#include "Graphics.h"
#include "HealthComponent.h"
#include "PhysicComponent.h"
#include "entt/entt.hpp"
#include "HashStringDataBase.h"
#include "GameplayTags.h"
class HealthSystem
{
public:
	void Update(entt::DefaultRegistry& ECSEngine)
	{
		ECSEngine.view<HealthComponent>().each([&ECSEngine](auto entity, HealthComponent &Health) {
			if (Health.curHealth <= 0)
			{
				ECSEngine.assign<DeathTag>(entity);
				return;
			}

			if (Health.curHealth >= 100)
			{
				ECSEngine.assign<DeathTag>(entity);
				ECSEngine.assign<SpawnEnemyInfo>(entity);
			}
		});
	}
};