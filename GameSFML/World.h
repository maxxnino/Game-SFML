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
	void AddEnemy(entt::DefaultRegistry& ECS, b2Vec2 position, b2Vec2 linerVel)
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position = position;
		bodyDef.linearVelocity = linerVel;
		const float size = 1.3f;
		b2CircleShape circle;
		circle.m_radius = size;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circle;
		fixtureDef.filter.categoryBits = CollisionFillter::ENEMY;
		fixtureDef.filter.maskBits = CollisionFillter::PLAYER | CollisionFillter::STATIC | CollisionFillter::BULLET;
		//fixtureDef.isSensor = isSensor;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.restitution = 1.0f;

		auto entity = ECS.create();
		ECS.assign<HealthComponent>(entity, 50.0f, 50.0f);

		//sprite
		{
			auto& sprite = ECS.assign<sf::Sprite>(entity, Locator::Codex::ref().GetTexture(Database::TEnemy));
			const auto textSize = sf::Vector2f(0.5f * (float)sprite.getTexture()->getSize().x, 0.5f * (float)sprite.getTexture()->getSize().y);
			sprite.setOrigin(textSize);
		}

		ECS.assign<PhysicDebug>(entity);
		ECS.assign<PhysicComponent>(entity, entity, bodyDef, fixtureDef);
		ECS.assign<CollisionRespondComponent>(entity).myDelegate.connect<&CollisionRespond::Enemy>();

		//Health Bar
		{
			auto entityUI = UIFactory::GetEntity(entity, ECS);
			auto& progressBar = ECS.assign<ProgressiveBarComponent>(entityUI, sf::Vector2f(100.0f, 15.0f));
			progressBar.colorBase = sf::Color::Blue;
			ECS.assign<WorldBaseUI>(entityUI, entity);
			ECS.assign<UpdateWorldBaseUIComponent>(entityUI).myDelegate.connect<&UpdateUI::WorldBaseHealthBar>();
		}
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

		ECS.assign<TransitionStateComponent>(entity).myDelegate.connect<&UpdateState::Player>();
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
		ECS.assign<CollisionRespondComponent>(entity).myDelegate.connect<&CollisionRespond::Player>();

		//health text
		{
			auto entityUI = UIFactory::GetEntity(entity, ECS);
			const auto& textFont = Locator::Codex::ref().GetFont(Database::FontSplatch);
			ECS.assign<sf::Text>(entityUI, "Health: ", textFont, 50);
			ECS.assign<ScreenBaseUI>(entityUI, entity, sf::Vector2f(-640.0f, -360.0f));
			ECS.assign<UpdateScreenBaseUIComponent>(entityUI).myDelegate.connect<&UpdateUI::ScreenBaseHealthText>();
		}
		

		//Health Bar
		{
			auto entityUI = UIFactory::GetEntity(entity, ECS);
			auto& progressBar = ECS.assign<ProgressiveBarComponent>(entityUI, sf::Vector2f(400.0f, 55.0f));
			ECS.assign<ScreenBaseUI>(entityUI, entity, sf::Vector2f(-640.0f, -360.0f));
			ECS.assign<UpdateScreenBaseUIComponent>(entityUI).myDelegate.connect<&UpdateUI::ScreenBaseHealthBar>();
		}
	}
private:
	void SignalComponent(entt::DefaultRegistry& ECS)
	{
		ECS.destruction<OwnedUIComponent>().connect<&UpdateUI::DeleteOwnedUIComponent>();
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
		AddECSSystem(std::make_unique<SpawnEnemySystem>());
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
		for (size_t i = 0; i < 500; i++)
		{
			AddEnemy(ECS, b2Vec2(pos(rng), pos(rng)), b2Vec2(speed(rng), speed(rng)));
		}

	}
private:
	std::vector<std::unique_ptr<ISystemECS>> ecsSystems;
	std::vector<std::unique_ptr<IDrawSystem>> drawSystems;
};