#pragma once
#include "Codex.h"
#include "HashStringDataBase.h"
#include "Box2DContactListener.h"
#include "RenderSpriteSystem.h"
#include "HealthSystem.h"
#include "SpawnAndCleanDeathSystem.h"
#include "AnimationSystem.h"
#include <random>
class World
{
public:
	World()
	{
		InitServiceLocator();

		AddWall(b2Vec2(32.0f,-18.0f),b2Vec2(32.0f, 18.0f));
		AddWall(b2Vec2(32.0f, 18.0f), b2Vec2(-32.0f, 18.0f));
		AddWall(b2Vec2(-32.0f, 18.0f), b2Vec2(-32.0f, -18.0f));
		AddWall(b2Vec2(-32.0f, -18.0f), b2Vec2(32.0f, -18.0f));

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;

		b2CircleShape circle;
		circle.m_radius = 1.5f;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circle;
		fixtureDef.filter.categoryBits = CollisionFillter::PLAYER;
		//fixtureDef.filter.maskBits = CollisionFillter::ENEMY | CollisionFillter::STATIC;
		//fixtureDef.isSensor = isSensor;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.restitution = 1.0f;

		b2FixtureDef fixtureDef1;
		fixtureDef1.shape = &circle;
		fixtureDef1.filter.categoryBits = CollisionFillter::ENEMY;
		//fixtureDef1.filter.maskBits = CollisionFillter::PLAYER | CollisionFillter::STATIC;
		//fixtureDef.isSensor = isSensor;
		fixtureDef1.density = 1.0f;
		fixtureDef1.friction = 0.0f;
		fixtureDef1.restitution = 1.0f;

		std::uniform_int_distribution<int> rangeID(0, 10);
		std::uniform_real_distribution<float> rangeX(-30.0f, 30.0f);
		std::uniform_real_distribution<float> rangeY(-15.0f, 15.0f);

		auto& rng = Locator::Random::ref();
		auto& ECS = Locator::ECS::ref();

		for (size_t i = 0; i < 100; i++)
		{
			auto entity = ECS.create();
			auto& sprite = ECS.assign<sf::Sprite>(entity);
			ECS.assign<HealthComponent>(entity, 50.0f);
			sprite.setTexture(Codex::GetTexture(Database::TEnemy01));
			const auto size = sprite.getTexture()->getSize();
			sprite.setOrigin((float)size.x / 2.0f, (float)size.y / 2.0f);
			bodyDef.position = b2Vec2(rangeX(rng), rangeY(rng));
			bodyDef.linearVelocity = b2Vec2(rangeX(rng), rangeX(rng));
			if (rangeID(rng) > 5)
			{
				ECS.assign<PhysicComponent>(entity, entity, bodyDef, fixtureDef);
				continue;
			}
			ECS.assign<PhysicComponent>(entity, entity, bodyDef, fixtureDef1);
		}
	}
	void Update(float dt)
	{
		Locator::Physic::ref().Step(dt, 4, 2);
		healthSystem.Update();
		spawnEnemySystem.Update();
		cleanDeadSystem.Update();
		animationSystem.Update(dt);
	}
	void Draw()
	{
		renderSystem.Draw();
	}
	
private:
	void InitServiceLocator()
	{
		Locator::Random::set(std::random_device{}());

		Locator::Physic::set(b2Vec2(0.0f, 0.0f));
		static Box2DContactListener mrListener;
		Locator::Physic::ref().SetContactListener(&mrListener);

		Locator::ECS::set();
	}
	void AddWall(b2Vec2 p1, b2Vec2 p2)
	{
		const auto entity = Locator::ECS::ref().create();
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(0, 0);

		b2EdgeShape edgeShape;
		edgeShape.Set(p1, p2);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &edgeShape;

		Locator::ECS::ref().assign<PhysicComponent>(entity, entity, bodyDef, fixtureDef);
	}
private:
	RenderSpriteSystem renderSystem;
	HealthSystem healthSystem;
	SpawnEnemySystem spawnEnemySystem;
	CleanDeadSystem cleanDeadSystem;
	AnimationSystem animationSystem;
};