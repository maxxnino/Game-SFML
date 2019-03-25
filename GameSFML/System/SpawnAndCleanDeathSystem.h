#pragma once
#include "../System/ISystemECS.h"
#include "../Component/GameplayTags.h"
#include "../Locator.h"
class CleanDeadSystem : public ISystemECS
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		ECS.destroy<DeathTag>();
	}
};
class SpawnEnemySystem : public ISystemECS
{
public:
	/**
	* Todo: add when spawn animationComponent...
	*/
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		ECS.view<SpawnEnemyInfo, PhysicComponent, sf::Sprite>().each([&ECS](auto entity,
			auto&, PhysicComponent& physicCom, sf::Sprite& sprite) {

			auto newEntity = ECS.create();
			ECS.assign<HealthComponent>(newEntity, 50.0f);
			if (ECS.has<AnimationComponent>(entity))
			{
				ECS.assign<AnimationComponent>(newEntity, ECS.get<AnimationComponent>(entity));
			}
			
			ECS.assign<sf::Sprite>(newEntity, sprite);

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
				ECS.assign<PhysicComponent>(newEntity, newEntity, bodyDef, fixtureDef);
			}
		});
		ECS.reset<SpawnEnemyInfo>();
	}
};