#pragma once
#include "GameplayTags.h"
#include "entt/entt.hpp"
#include "Locator.h"
class CleanDeadSystem
{
public:
	void Update()
	{
		Locator::ECS::ref().destroy<DeathTag>();
	}
};
class SpawnEnemySystem
{
public:
	void Update()
	{
		auto& ECSEngine = Locator::ECS::ref();
		ECSEngine.view<SpawnEnemyInfo, PhysicComponent, sf::Sprite>().each([&ECSEngine](auto entity,
			auto&, PhysicComponent& physicCom, sf::Sprite& sprite) {

			auto newEntity = ECSEngine.create();
			ECSEngine.assign<HealthComponent>(newEntity, 50.0f);
			if (ECSEngine.has<AnimationComponent>(entity))
			{
				ECSEngine.assign<AnimationComponent>(newEntity, ECSEngine.get<AnimationComponent>(entity));
			}
			
			ECSEngine.assign<sf::Sprite>(newEntity, sprite);

			{
				b2BodyDef bodyDef;
				b2FixtureDef fixtureDef;
				bodyDef.type = physicCom.body->GetType();
				bodyDef.position = physicCom.body->GetPosition();
				fixtureDef.shape = physicCom.body->GetFixtureList()->GetShape();
				fixtureDef.filter = physicCom.body->GetFixtureList()->GetFilterData();
				fixtureDef.density = physicCom.body->GetFixtureList()->GetDensity();
				fixtureDef.friction = physicCom.body->GetFixtureList()->GetFriction();
				fixtureDef.restitution = physicCom.body->GetFixtureList()->GetRestitution();
				ECSEngine.assign<PhysicComponent>(newEntity, newEntity, bodyDef, fixtureDef);
			}
		});
		ECSEngine.reset<SpawnEnemyInfo>();
	}
};