#pragma once
#include "Codex.h"
#include "HashStringDataBase.h"
#include "Box2DContactListener.h"
#include "RenderSpriteSystem.h"
#include "HealthSystem.h"
#include "SpawnAndCleanDeathSystem.h"
#include "AnimationSystem.h"
#include "MoveCameraSystem.h"
#include <random>
class World
{
public:
	World()
	{
		InitServiceLocator();

		AddWall(b2Vec2(100.0f,-50.0f),b2Vec2(100.0f, 50.0f));
		AddWall(b2Vec2(100.0f, 50.0f), b2Vec2(-100.0f, 50.0f));
		AddWall(b2Vec2(-100.0f, 50.0f), b2Vec2(-100.0f, -50.0f));
		AddWall(b2Vec2(-100.0f, -50.0f), b2Vec2(100.0f, -50.0f));

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		const float size = 0.4f;
		b2CircleShape circle;
		circle.m_radius = size;

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
		std::uniform_real_distribution<float> rangeX(-98.0f, 98.f);
		std::uniform_real_distribution<float> rangeY(-48.0f, 48.0f);

		auto& rng = Locator::Random::ref();
		auto& ECS = Locator::ECS::ref();
		auto shareEntity = ECS.create();
		
		{
			//share animation
			auto& ShareAnimation = ECS.assign<AnimationShareComponent>(shareEntity);
			for (int i = 0; i < 8; i++)
			{
				ShareAnimation.frames.emplace_back(sf::IntRect(i * 30, 0, 30, 30));
			}
		}
		
		for (size_t i = 0; i < 5000; i++)
		{
			auto entity = ECS.create();
			{
				//sprite
				auto& sprite = ECS.assign<sf::Sprite>(entity);
				sprite.setTexture(Codex::GetTexture(Database::TBullet01));
				sprite.setOrigin(2.0f * size * Locator::Graphic::ref().scalePixel, 2.0f * size * Locator::Graphic::ref().scalePixel);
			}
			
			ECS.assign<HealthComponent>(entity, 50.0f);
			ECS.assign<AnimationComponent>(entity, 0.1f, 0.0f,0u,8u, shareEntity);
			ECS.assign<UpdateAnimation>(entity, shareEntity, 0u);
			
			bodyDef.position = b2Vec2(rangeX(rng), rangeY(rng));
			bodyDef.linearVelocity = b2Vec2(0.5f * rangeY(rng), 0.5f * rangeY(rng));
			if (rangeID(rng) > 5)
			{
				ECS.assign<PhysicComponent>(entity, entity, bodyDef, fixtureDef);
				continue;
			}
			ECS.assign<PhysicComponent>(entity, entity, bodyDef, fixtureDef1);
		}
		WarmUp();
	}
	void Update(float dt)
	{
		Locator::Physic::ref().Step(dt, 4, 2);
		healthSystem.Update();
		spawnEnemySystem.Update();
		cleanDeadSystem.Update();
		moveCameraSystem.Update();
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
	void WarmUp()
	{
		animationSystem.WarmUp();
	}
private:
	RenderSpriteSystem renderSystem;
	HealthSystem healthSystem;
	SpawnEnemySystem spawnEnemySystem;
	CleanDeadSystem cleanDeadSystem;
	AnimationSystem animationSystem;
	MoveCameraSystem moveCameraSystem;
};