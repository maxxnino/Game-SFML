#pragma once
#include "HashStringDataBase.h"
#include "Box2DContactListener.h"
#include "SystemInclude.h"
#include "MyFreeFunction.h"
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
		const float size = 1.3f;
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
		
		for (size_t i = 0; i < 100; i++)
		{
			auto entity = ECS.create();
			ECS.assign<HealthComponent>(entity, 50.0f);
			ECS.assign<PlayerControllerComponent>(entity);

			auto& animation = ECS.assign<AnimationComponent>(entity, 
				Locator::Codex::ref().GetAnimation(Database::PlayerAnimation), 
				ECS.assign<PlayerStateComponent>(entity).state);

			ECS.assign<TransitionStateComponent>(entity).transitionRule.sink().connect<&Maxx::PlayerUpdateState>();
			//sprite
			{
				auto& sprite = ECS.assign<sf::Sprite>(entity);
				sprite.setTexture(*animation.resource->texture);
				const auto textSize = 0.5f * sf::Vector2f((float)animation.resource->tileWidth, (float)animation.resource->tileHeight);
				sprite.setOrigin(textSize);
			}
			bodyDef.position = b2Vec2(rangeX(rng), rangeY(rng));
			bodyDef.linearVelocity = b2Vec2(0.5f * rangeY(rng), 0.5f * rangeY(rng));

			ECS.assign<PhysicDebug>(entity);
			if (rangeID(rng) > 5)
			{
				ECS.assign<PhysicComponent>(entity, entity, bodyDef, fixtureDef);
			}
			else
			{
				ECS.assign<PhysicComponent>(entity, entity, bodyDef, fixtureDef1);
			}
		}
	}
	void Update(float dt)
	{
		Locator::Physic::ref().Step(dt, 4, 2);
		if (Locator::ECS::empty()) return;
		
		for (auto& sys : ecsSystems)
		{
			sys->Update(Locator::ECS::ref(), dt);
		}
	}
	void Draw()const
	{
		if (Locator::Graphic::empty()) return;
		auto& gfx = Locator::Graphic::ref();
		for (auto sys : drawSystems)
		{
			if (sys != nullptr)
			{
				sys->Draw(gfx);
			}
		}
		//debugSystem.Draw(gfx);
	}
	void AddECSSystem(std::unique_ptr<ISystemECS> newSystem)
	{
		ecsSystems.emplace_back(std::move(newSystem));
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

		Locator::Grid::set();
		Locator::Grid::ref().LoadFromFile(Locator::Codex::ref(), Locator::ECS::ref());
	}
	void InitSystem()
	{
		AddECSSystem(std::make_unique<SpawnStaticObjectSystem>());
		AddECSSystem(std::make_unique<HealthSystem>());
		AddECSSystem(std::make_unique<SpawnEnemySystem>());
		AddECSSystem(std::make_unique<CleanDeadSystem>());
		AddECSSystem(std::make_unique<MoveCameraSystem>());
		AddECSSystem(std::make_unique<CullingSystem>());
		AddECSSystem(std::make_unique<PlayerControllerSystem>());
		AddECSSystem(std::make_unique<TransitionStateSystem>());
		AddECSSystem(std::make_unique<AnimationSystem>());
		//render Grid should be before render sprite and after move camera;
		{
			auto newSystem = std::make_unique<RenderGridSystem>();
			drawSystems.emplace_back(static_cast<IDrawSystem*>(newSystem.get()));
			ecsSystems.emplace_back(std::move(newSystem));
			
		}
		//render Sprite should be the last
		{
			auto newSystem = std::make_unique<RenderSpriteSystem>();
			drawSystems.emplace_back(static_cast<IDrawSystem*>(newSystem.get()));
			ecsSystems.emplace_back(std::move(newSystem));
		}
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
	std::vector<std::unique_ptr<ISystemECS>> ecsSystems;
	std::vector<IDrawSystem*> drawSystems;
	DrawDebugSystem debugSystem;
};