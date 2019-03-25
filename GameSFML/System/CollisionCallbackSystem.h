#pragma once
#include "Locator.h"
#include "Component/PhysicComponent.h"
#include "Component/HealthComponent.h"
#include "System/ISystemECS.h"

class CollisionCallbackSystem : public ISystemECS
{
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		auto view = ECS.view<CollisionCallbackComponent>();

		std::for_each(std::execution::par, view.begin(), view.end(), [&ECS](auto entity) {
			auto& callbackData = ECS.get<CollisionCallbackComponent>(entity);

			switch (callbackData.selfCatagory)
			{
			case CollisionFillter::ENEMY:
			{
				for (auto& other : callbackData.others)
				{
					if (other.second == CollisionFillter::ENEMY) continue;

					if (other.second == CollisionFillter::PLAYER)
					{
						if (ECS.has<HealthComponent>(entity))
						{
							ECS.get<HealthComponent>(entity).curHealth -= 5.0f;
						}
						if (ECS.has<HealthComponent>(other.first))
						{
							ECS.get<HealthComponent>(other.first).curHealth -= 5.0f;
						}
					}
				}
			}
			break;
			case CollisionFillter::PLAYER:
			{
				for (auto& other : callbackData.others)
				{
					if (other.second == CollisionFillter::PLAYER) continue;

					if (other.second == CollisionFillter::ENEMY)
					{
						if (ECS.has<HealthComponent>(entity))
						{
							ECS.get<HealthComponent>(entity).curHealth -= 5.0f;
						}
						if (ECS.has<HealthComponent>(other.first))
						{
							ECS.get<HealthComponent>(other.first).curHealth -= 5.0f;
						}
					}
				}
			}
			break;
			default:
				break;
			}
		});

		ECS.reset<CollisionCallbackComponent>();
	}
};