#pragma once
#include "../Component/PhysicComponent.h"
#include "../Component/HealthComponent.h"
#include "entt/entt.hpp"
struct CollisionCallbackData
{
	// First: other entity - Second: categotyMask
	std::vector<std::pair<uint32_t, uint16>> others;
};
struct CollisionRespondComponent
{
	//this will use by CollisionRespondSystem, and this system will use multi-thread don't remove or add new Component
	entt::Delegate<void(uint32_t, entt::DefaultRegistry&)> myDelegate;
};

struct CollisionRespond
{
	static void Player(uint32_t entity, entt::DefaultRegistry& ECS)
	{
		auto& callbackData = ECS.get<CollisionCallbackData>(entity);

		for (auto& other : callbackData.others)
		{
			if (other.second == CollisionFillter::PLAYER) continue;

			if (other.second == CollisionFillter::ENEMY)
			{
				if (ECS.has<HealthComponent>(entity))
				{
					ECS.get<HealthComponent>(entity).curHealth -= 1.0f;
				}
			}
		}
	}
	static void Enemy(uint32_t entity, entt::DefaultRegistry& ECS)
	{
		auto& callbackData = ECS.get<CollisionCallbackData>(entity);

		for (auto& other : callbackData.others)
		{
			if (other.second == CollisionFillter::ENEMY) continue;

			if (other.second == CollisionFillter::PLAYER)
			{
				if (ECS.has<HealthComponent>(entity))
				{
					ECS.get<HealthComponent>(entity).curHealth -= 5.0f;
				}
			}
		}
	}
	static void Bullet(uint32_t entity, entt::DefaultRegistry& ECS)
	{
		auto& callbackData = ECS.get<CollisionCallbackData>(entity);

		for (auto& other : callbackData.others)
		{
			if (other.second == CollisionFillter::BULLET) continue;

			if (other.second == CollisionFillter::ENEMY)
			{
				if (ECS.has<HealthComponent>(entity))
				{
					ECS.get<HealthComponent>(entity).curHealth = 0.0f;
				}
				if (ECS.has<HealthComponent>(other.first))
				{
					ECS.get<HealthComponent>(other.first).curHealth -= 20.0f;
				}
			}
		}
	}
};