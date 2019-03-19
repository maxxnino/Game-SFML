#pragma once
#include "HashStringDataBase.h"
#include "Box2DContactListener.h"
#include "RenderSpriteSystem.h"
#include "HealthSystem.h"
#include "SpawnAndCleanDeathSystem.h"
#include "AnimationSystem.h"
#include "MoveCameraSystem.h"
#include "PlayerControllerSystem.h"
#include <random>
class World
{
public:
	World()
	{
		InitServiceLocator();
		InitSystem();
		AddWall(b2Vec2(100.0f,-50.0f),b2Vec2(100.0f, 50.0f));
		AddWall(b2Vec2(100.0f, 50.0f), b2Vec2(-100.0f, 50.0f));
		AddWall(b2Vec2(-100.0f, 50.0f), b2Vec2(-100.0f, -50.0f));
		AddWall(b2Vec2(-100.0f, -50.0f), b2Vec2(100.0f, -50.0f));

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		const float size = 3.0f;
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
		
		for (size_t i = 0; i < 500; i++)
		{
			auto entity = ECS.create();
			{
				//sprite
				auto& sprite = ECS.assign<sf::Sprite>(entity);
				sprite.setTexture(Locator::Codex::ref().GetTexture(Database::TPlayer01));
				sprite.setOrigin(2.0f * size * Locator::Graphic::ref().scalePixel, 2.0f * size * Locator::Graphic::ref().scalePixel);
			}
			ECS.assign<HealthComponent>(entity, 50.0f);
			ECS.assign<AnimationComponent>(entity, Locator::Codex::ref().GetFramesRect("PlayerDown"_hs));
			ECS.assign<PlayerController>(entity);
			ECS.assign<AnimationState>(entity) = AnimationState::WALKING;
			ECS.assign<Direction>(entity) = Direction::DOWN;
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
		if (Locator::ECS::empty()) return;
		
		for (auto& sys : systems)
		{
			sys->Update(Locator::ECS::ref(), dt);
		}
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

		Locator::Codex::set();
	}
	void InitSystem()
	{
		systems.emplace_back(std::make_unique<HealthSystem>());
		systems.emplace_back(std::make_unique<SpawnEnemySystem>());
		systems.emplace_back(std::make_unique<CleanDeadSystem>());
		systems.emplace_back(std::make_unique<MoveCameraSystem>());
		systems.emplace_back(std::make_unique<PlayerControllerSystem>());
		systems.emplace_back(std::make_unique<AnimationSystem>());
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
		
	}
private:
	std::vector<std::unique_ptr<ISystem>> systems;
	RenderSpriteSystem renderSystem;
};