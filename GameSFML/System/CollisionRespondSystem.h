#pragma once
#include "../Locator.h"
#include "../Component/CollisionRespondComponent.h"
#include "../Component/PhysicComponent.h"
#include "../Component/HealthComponent.h"
#include "../System/ISystemECS.h"

class CollisionRespondSystem : public ISystemECS
{
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		auto view = ECS.view<CollisionCallbackData, CollisionRespondComponent>();
		std::for_each(std::execution::par, view.begin(), view.end(), [&ECS](auto entity) {
			auto& respond = ECS.get<CollisionRespondComponent>(entity);
			if (respond.myDelegate.empty()) return;

			respond.myDelegate(entity, ECS);
		});

		ECS.reset<CollisionCallbackData>();
	}
};