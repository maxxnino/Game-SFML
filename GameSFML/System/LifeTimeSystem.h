#pragma once
#include "../System/ISystemECS.h"
#include "../Component/GameplayTags.h"
class LifeTimeSystem : public ISystemECS
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt)
	{
		ECS.view<LifeTimeComponent>().each([&ECS,dt](auto entity, LifeTimeComponent& lifeTime) {
			lifeTime.curTime += dt;
			if (lifeTime.curTime >= lifeTime.maxLifeTime)
			{
				ECS.destroy(entity);
			}
		});
	}
};