#pragma once
#include "PhysicComponent.h"
#include "HealthComponent.h"
#include "Locator.h"
class Box2DContactListener final : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact) final
	{
		auto categoryBits1 = contact->GetFixtureA()->GetFilterData().categoryBits;
		auto entity1 = contact->GetFixtureA()->GetBody()->GetUserEntity();

		auto categoryBits2 = contact->GetFixtureB()->GetFilterData().categoryBits;
		auto entity2 = contact->GetFixtureB()->GetBody()->GetUserEntity();

		if (categoryBits1 == categoryBits2) return;

		switch (categoryBits1 | categoryBits2)
		{
		case CollisionFillter::ENEMY | CollisionFillter::PLAYER:
		{
			if (categoryBits1 == CollisionFillter::ENEMY)
			{
				Locator::ECS::ref().get<HealthComponent>(entity1).curHealth += 10;
				Locator::ECS::ref().get<HealthComponent>(entity2).curHealth -= 10;
				break;
			}
			Locator::ECS::ref().get<HealthComponent>(entity2).curHealth += 10;
			Locator::ECS::ref().get<HealthComponent>(entity1).curHealth -= 10;
			break;
		}
		default:
			break;
		}
	}
};
