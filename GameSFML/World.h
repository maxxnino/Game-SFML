#pragma once
#include "Mouse.h"
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
		box2DEngine = std::make_unique<b2World>(b2Vec2(0.0f, 0.0f));
		static Box2DContactListener mrListener{ ECSEngine };
		box2DEngine->SetContactListener(&mrListener);

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
		for (size_t i = 0; i < 50; i++)
		{
			auto entity = ECSEngine.create();
			auto& sprite = ECSEngine.assign<sf::Sprite>(entity);
			ECSEngine.assign<HealthComponent>(entity, 50.0f);
			sprite.setTexture(Codex::GetTexture(Database::TEnemy01));
			bodyDef.position = b2Vec2(rangeX(rng), rangeY(rng));
			bodyDef.linearVelocity = b2Vec2(rangeX(rng), rangeY(rng));
			if (rangeID(rng) == 0)
			{
				ECSEngine.assign<PhysicComponent>(entity, entity, *box2DEngine, bodyDef, fixtureDef);
				continue;
			}
			ECSEngine.assign<PhysicComponent>(entity, entity, *box2DEngine, bodyDef, fixtureDef1);	
		}
	}
	void Update(float dt, Keyboard& kbd, Mouse& mouse, Graphics& gfx)
	{
		box2DEngine->Step(dt, 4, 2);
		healthSystem.Update(ECSEngine);
		spawnEnemySystem.Update(ECSEngine,*box2DEngine);
		cleanDeadSystem.Update(ECSEngine);
	}
	void Draw(Graphics& gfx)
	{
		renderSystem.Draw(ECSEngine, gfx);
	}
private:
	void AddWall(b2Vec2 p1, b2Vec2 p2)
	{
		auto entity = ECSEngine.create();
		PhysicComponent& physic = ECSEngine.assign<PhysicComponent>(entity);
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_staticBody; //change body type
			bodyDef.position.Set(0, 0); //middle, bottom
			physic.body = { box2DEngine->CreateBody(&bodyDef),[this](b2Body* pBody) {box2DEngine->DestroyBody(pBody); } };
		}

		{
			//shape
			b2EdgeShape edgeShape;
			edgeShape.Set(p1, p2);

			//fixture
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &edgeShape;
			//collision fillter
			//fixtureDef.filter.categoryBits = categoryBits;
			//fixtureDef.filter.maskBits = maskBits;
			physic.body->CreateFixture(&fixtureDef);
		}
	}
private:
	/*void CreateEnemyArcheType()
	{
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;

			b2PolygonShape dynamicBox;
			dynamicBox.SetAsBox(2.0f, 2.0f);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicBox;
			fixtureDef.filter.categoryBits = CollisionFillter::PLAYER;
			fixtureDef.filter.maskBits = CollisionFillter::ENEMY | CollisionFillter::BORDER;
			fixtureDef.isSensor = isSensor;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.0f;
			fixtureDef.restitution = 1.0f;

			enemyArcheType.set<PhysicComponent>(*box2DEngine, bodyDef, fixtureDef);
		}
		enemyArcheType.set<HealthComponent>(50.0f);
		enemyArcheType.set<sf::Sprite>();
	}*/
private:
	std::unique_ptr<b2World> box2DEngine;
	entt::DefaultRegistry ECSEngine;
	entt::DefaultPrototype enemyArcheType{ ECSEngine };
	RenderSpriteSystem renderSystem;
	HealthSystem healthSystem;
	SpawnEnemySystem spawnEnemySystem;
	CleanDeadSystem cleanDeadSystem;
	
	
	std::mt19937 rng = std::mt19937(std::random_device{}());
};