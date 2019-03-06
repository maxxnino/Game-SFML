#pragma once
#include "GameplayTags.h"
#include "entt/entt.hpp"
#include "Box2D/Box2D.h"
class CleanDeadSystem
{
public:
	void Update(entt::DefaultRegistry& ECSEngine)
	{
		ECSEngine.destroy<DeathTag>();
	}
};
class SpawnEnemySystem
{
public:
	void Update(entt::DefaultRegistry& ECSEngine, b2World& box2DEngine)
	{
		PhysicComponent aaa;
		ECSEngine.view<SpawnEnemyInfo>().each([&ECSEngine,&box2DEngine](auto entity, SpawnEnemyInfo & info) {
			ECSEngine.assign<HealthComponent>(entity, 50.0f);
			auto& physic = ECSEngine.assign<PhysicComponent>(entity, entity, box2DEngine, info.bodyDef, info.fixtureDef);
			physic.body->SetTransform(info.pos,0.0f);
			sf::Sprite& sprite =  ECSEngine.assign<sf::Sprite>(entity);
			sprite.setTexture(*info.sprite.getTexture());
		});
	}
};