#pragma once
#include "../System/ISystemECS.h"
#include "../Component/GameplayTags.h"
#include "../Component/SpawnComponent.h"
#include "../Locator.h"
class CleanDeadSystem : public ISystemECS
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		ECS.view<DeathTag, NotifyOnDead>().each([&ECS, dt](auto entity, auto&, NotifyOnDead& notifier) {
			notifier.mySignal.publish(entity, ECS);
			ECS.destroy(entity);
		});
		ECS.destroy<DeathTag>();
	}
};
class SpawnSystem : public ISystemECS
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		ECS.view<SpawnComponent, UpdateSpawnComponent>().each([&ECS, dt](auto entity, SpawnComponent& spawn, UpdateSpawnComponent& update) {
			if (!spawn.bIsEnable) return;

			spawn.curTime += dt;
			while (spawn.curTime >= spawn.interval)
			{
				spawn.curTime -= spawn.interval;
				update.myDelegate(entity, ECS);
			}
		});
	}
};