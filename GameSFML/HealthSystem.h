#pragma once
#include "Graphics.h"
#include "HealthComponent.h"
#include "PhysicComponent.h"
#include "Locator.h"
#include "HashStringDataBase.h"
#include "GameplayTags.h"
class HealthSystem
{
public:
	void Update()
	{
		Locator::ECS::ref().view<HealthComponent>().each([](auto entity, HealthComponent &Health) {
			if (Health.curHealth <= 0)
			{
				Locator::ECS::ref().assign<DeathTag>(entity);
				return;
			}

			if (Health.curHealth >= 100)
			{
				Locator::ECS::ref().assign<DeathTag>(entity);
				Locator::ECS::ref().assign<SpawnEnemyInfo>(entity);
			}
		});
	}
};