#pragma once
#include "Mouse.h"
#include "Locator.h"
#include "Codex.h"
#include "HashStringDataBase.h"
#include "RenderSpriteSystem.h"
#include "HealthSystem.h"
#include "Box2DContactListener.h"
#include "SpawnAndCleanDeathSystem.h"
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

		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(2.0f, 2.0f);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.filter.categoryBits = CollisionFillter::PLAYER;
		fixtureDef.filter.maskBits = CollisionFillter::ENEMY | CollisionFillter::BORDER;
		//fixtureDef.isSensor = isSensor;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.restitution = 1.0f;

		b2FixtureDef fixtureDef1;
		fixtureDef1.shape = &dynamicBox;
		fixtureDef1.filter.categoryBits = CollisionFillter::ENEMY;
		fixtureDef1.filter.maskBits = CollisionFillter::PLAYER | CollisionFillter::BORDER;
		//fixtureDef.isSensor = isSensor;
		fixtureDef1.density = 1.0f;
		fixtureDef1.friction = 0.0f;
		fixtureDef1.restitution = 1.0f;

		std::uniform_int_distribution<int> rangeID(0, 1);
		std::uniform_real_distribution<float> rangeX(-30.0f, 30.0f);
		std::uniform_real_distribution<float> rangeY(-15.0f, 15.0f);

		auto& rng = Locator::Random::ref();
		auto& ECS = Locator::ECS::ref();

		for (size_t i = 0; i < 50; i++)
		{
			auto entity = ECS.create();
			auto& sprite = ECS.assign<sf::Sprite>(entity);
			ECS.assign<HealthComponent>(entity, 50.0f);
			sprite.setTexture(Codex::GetTexture(Database::TEnemy01));
			bodyDef.position = b2Vec2(rangeX(rng), rangeY(rng));
			bodyDef.linearVelocity = b2Vec2(rangeX(rng), rangeY(rng));
			if (rangeID(rng) == 0)
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
		Locator::ECS::set();
		static Box2DContactListener mrListener{ Locator::ECS::ref() };
		Locator::Physic::ref().SetContactListener(&mrListener);
	}
	void AddWall(b2Vec2 p1, b2Vec2 p2)
	{
		auto entity = Locator::ECS::ref().create();
		PhysicComponent& physic = Locator::ECS::ref().assign<PhysicComponent>(entity);
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_staticBody;
			bodyDef.position.Set(0, 0);
			physic.body = { Locator::Physic::ref().CreateBody(&bodyDef),[this](b2Body* pBody) {Locator::Physic::ref().DestroyBody(pBody); } };
		}

		{
			b2EdgeShape edgeShape;
			edgeShape.Set(p1, p2);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &edgeShape;
			physic.body->CreateFixture(&fixtureDef);
		}
	}
private:
	RenderSpriteSystem renderSystem;
	HealthSystem healthSystem;
	SpawnEnemySystem spawnEnemySystem;
	CleanDeadSystem cleanDeadSystem;
};