#pragma once
#include "HashStringDataBase.h"
#include "Box2DContactListener.h"
#include "SystemInclude.h"
#include <random>
class World
{
public:
	World()
	{
		InitServiceLocator();
		InitSystem();
		TestSomething();
		
	}
	void Update(float dt)
	{
		if (Locator::ECS::empty()) return;
		
		for (auto& sys : ecsSystems)
		{
			sys->Update(Locator::ECS::ref(), dt);
		}
	}
	void Draw()const
	{
		if (Locator::Graphic::empty() || Locator::ECS::empty()) return;
		auto& gfx = Locator::Graphic::ref();
		auto& ECS = Locator::ECS::ref();
		for (auto& sys : drawSystems)
		{
			sys->Draw(gfx, ECS);
		}
		
	}
	void AddECSSystem(std::unique_ptr<ISystemECS> newSystem)
	{
		ecsSystems.emplace_back(std::move(newSystem));
	}
	void AddDrawSystem(std::unique_ptr<IDrawSystem> newSystem)
	{
		drawSystems.emplace_back(std::move(newSystem));
	}
	void AddSpawner(entt::DefaultRegistry& ECS, float worldSize)
	{
		auto entity = ECS.create();
		ECS.assign<SpawnComponent>(entity).interval = 1.0f;
		ECS.assign<SpawnCapacity>(entity).maxEntity = 20;
		ECS.assign<UpdateSpawnComponent>(entity).myDelegate.connect<UpdateSpawnComponent::Enemy>();

		if (Locator::Random::empty()) return;
		auto& rng = Locator::Random::ref();
		std::uniform_real_distribution<float> posRange(-worldSize, worldSize);
		std::uniform_real_distribution<float> speedRange(-20.0f, 20.0f);
		auto& spawnPos = ECS.assign<SpawnPosition>(entity);
		spawnPos.spawnPosition = b2Vec2(posRange(rng), posRange(rng));
		spawnPos.variationX = 5;
		spawnPos.variationX = 2;
		spawnPos.speed = b2Vec2(speedRange(rng), speedRange(rng));
	}
	void AddPlayer(entt::DefaultRegistry& ECS)
	{
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
		auto entity = ECS.create();
		ECS.assign<HealthComponent>(entity, 50.0f, 50.0f);
		ECS.assign<PlayerControllerComponent>(entity);

		auto& animation = ECS.assign<AnimationComponent>(entity,
			Locator::Codex::ref().GetAnimation(Database::PlayerAnimation),
			ECS.assign<PlayerStateComponent>(entity).state);

		ECS.assign<TransitionStateComponent>(entity).myDelegate.connect<&TransitionStateComponent::Player>();
		//sprite
		{
			auto& sprite = ECS.assign<sf::Sprite>(entity);
			sprite.setTexture(*animation.resource->texture);
			const auto textSize = 0.5f * sf::Vector2f((float)animation.resource->tileWidth, (float)animation.resource->tileHeight);
			sprite.setOrigin(textSize);
		}


		ECS.assign<PhysicDebug>(entity);
		ECS.assign<CameraTracking>(entity);
		ECS.assign<PhysicComponent>(entity, entity, bodyDef, fixtureDef);
		ECS.assign<CollisionRespondComponent>(entity).myDelegate.connect<&CollisionRespondComponent::Player>();

		//health text
		{
			auto entityUI = UIFactory::GetEntity(entity, ECS);
			const auto& textFont = Locator::Codex::ref().GetFont(Database::FontSplatch);
			ECS.assign<sf::Text>(entityUI, "Health: ", textFont, 50);
			ECS.assign<ScreenBaseUI>(entityUI, entity, sf::Vector2f(-640.0f, -360.0f));
			ECS.assign<UpdateScreenBaseUIComponent>(entityUI).myDelegate.connect<&UpdateScreenBaseUIComponent::HealthText>();
		}
		

		//Health Bar
		{
			auto entityUI = UIFactory::GetEntity(entity, ECS);
			auto& progressBar = ECS.assign<ProgressiveBarComponent>(entityUI, sf::Vector2f(400.0f, 55.0f));
			ECS.assign<ScreenBaseUI>(entityUI, entity, sf::Vector2f(-640.0f, -360.0f));
			ECS.assign<UpdateScreenBaseUIComponent>(entityUI).myDelegate.connect<&UpdateScreenBaseUIComponent::HealthBar>();
		}
	}
private:
	void SignalComponent(entt::DefaultRegistry& ECS)
	{
		ECS.destruction<OwnedUIComponent>().connect<&OwnedUIComponent::Destruction>();
	}
	void InitServiceLocator()
	{
		Locator::Random::set(std::random_device{}());
		Locator::ECS::set();
		SignalComponent(Locator::ECS::ref());

		Locator::Physic::set(b2Vec2(0.0f, 0.0f));
		if (!Locator::ECS::empty())
		{
			static Box2DContactListener mrListener{ Locator::ECS::ref() };
			Locator::Physic::ref().SetContactListener(&mrListener);
		}		

		Locator::Codex::set();
		Locator::Grid::set();
		Locator::Grid::ref().LoadFromFile(Locator::Codex::ref(), Locator::ECS::ref());
	}
	void InitSystem()
	{
		AddECSSystem(std::make_unique<SpawnStaticObjectSystem>());
		AddECSSystem(std::make_unique<LifeTimeSystem>());
		AddECSSystem(std::make_unique<PlayerControllerSystem>());
		AddECSSystem(std::make_unique<PlayerUpdateSystem>());
		AddECSSystem(std::make_unique<PhysicSystem>());
		AddECSSystem(std::make_unique<CollisionRespondSystem>());
		AddECSSystem(std::make_unique<HealthSystem>());
		AddECSSystem(std::make_unique<MoveCameraSystem>());
		AddECSSystem(std::make_unique<GridUpdateSystem>());
		AddECSSystem(std::make_unique<SpawnSystem>());
		AddECSSystem(std::make_unique<CleanDeadSystem>());
		AddECSSystem(std::make_unique<CullingSystem>());
		AddECSSystem(std::make_unique<UpdateScreenBaseUISystem>());
		AddECSSystem(std::make_unique<UpdateWorldBaseUISystem>());
		AddECSSystem(std::make_unique<TransitionStateSystem>());
		AddECSSystem(std::make_unique<AnimationSystem>());
		AddECSSystem(std::make_unique<SpirteUpdateSystem>());

		//render Grid should be before render sprite and after move camera;
		AddDrawSystem(std::make_unique<RenderGridSystem>());
		//render Sprite should be the last
		AddDrawSystem(std::make_unique<RenderSpriteSystem>());
		drawSystems.emplace_back(std::make_unique<DrawDebugSystem>());
		drawSystems.emplace_back(std::make_unique<RenderWorldBaseUISystem>());
		drawSystems.emplace_back(std::make_unique<RenderScreenBaseUISystem>());
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
	void TestSomething()
	{
		auto& ECS = Locator::ECS::ref();
		auto& rng = Locator::Random::ref();
		const float worldSize = 100.0f;
		AddWall(b2Vec2(worldSize, -worldSize), b2Vec2(worldSize, worldSize));
		AddWall(b2Vec2(worldSize, worldSize), b2Vec2(-worldSize, worldSize));
		AddWall(b2Vec2(-worldSize, worldSize), b2Vec2(-worldSize, -worldSize));
		AddWall(b2Vec2(-worldSize, -worldSize), b2Vec2(worldSize, -worldSize));

		AddPlayer(ECS);

		std::uniform_real_distribution<float> pos(-worldSize + 5.0f, worldSize - 5.0f);
		std::uniform_real_distribution<float> speed(-20.0f, 20.0f);
		for (size_t i = 0; i < 10; i++)
		{
			AddSpawner(ECS, worldSize - 10.0f);
		}
	}
private:
	std::vector<std::unique_ptr<ISystemECS>> ecsSystems;
	std::vector<std::unique_ptr<IDrawSystem>> drawSystems;
};