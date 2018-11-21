#pragma once
#include "entt/entt.hpp"
#include "CameraSystem.h"
#include "PlayerControlSystem.h"
#include "DrawSystem.h"
#include "AnimationSystem.h"
#include "PhysicSystem.h"
#include "Mouse.h"
#include "Codex.h"

class World
{
public:
	World()
	{
		box2DEngine = std::make_unique<b2World>(b2Vec2(0.0f, 0.0f));
		codex.AddTexture(HS::Enemy01, "Data\\Images\\Player_12_4_95_159.png");
		//codex.AddIntRect(HS::Enemy01, 0, 0, 30, 30, 4);
		codex.AddTexture(HS::Enemy02, "Data\\Images\\pro_04_30_30_8.png");
		//codex.AddIntRect(HS::Enemy02, 0, 0, 30, 30, 8);
		codex.AddTexture(HS::Enemy03, "Data\\Images\\pro_05_30_30_4.png");
		//codex.AddIntRect(HS::Enemy03, 0, 0, 30, 30, 4);
		codex.AddTexture(HS::Enemy04, "Data\\Images\\pro_07_30_30_4.png");
		//codex.AddIntRect(HS::Enemy04, 0, 0, 30, 30, 4);
	}
	void Update(float dt, Keyboard& kbd, Mouse& mouse, Graphics& gfx)
	{
		camSystem.Update(gfx.GetRenderWindow(), kbd, dt);
		controlSystem.Update(ECS, dt, kbd, mouse);
		animSystem.Update(ECS, dt);
		physicSystem.Update(*box2DEngine, dt);


		drawSystem.Update(ECS, gfx, *box2DEngine);
	}
	void Draw(Graphics& gfx)
	{
		drawSystem.Draw(ECS, gfx, codex);
	}
	void AddPlayer(const b2Vec2& pos)
	{
		auto entity = ECS.create();
		ECS.assign<entt::label<HS::InputComponent>>(entity);
		ECS.assign<sf::Sprite>(entity).setTexture(codex.GetTexture(HS::Enemy01));

		//amination
		{
			GroupAnimation& group = ECS.assign<GroupAnimation>(entity);
			group.iAnimation = 2;
			for (int collum = 0; collum < 4; collum++)
			{
				group.animations.emplace_back();
				group.animations[collum].holdTime = 0.1f;
				for (int row = 0; row < 12; row++)
				{
					group.animations[collum].frames.emplace_back(sf::IntRect(row * 95, collum * 159, 95, 159));
				}
			}
		}
		
		
		//physic
		{
			PhysicComponent& physic = ECS.assign<PhysicComponent>(entity);
			{
				b2BodyDef bodyDef;
				bodyDef.type = b2_dynamicBody;
				bodyDef.position.Set(pos.x, pos.y);
				physic.body = { box2DEngine->CreateBody(&bodyDef),[this](b2Body* pBody) {box2DEngine->DestroyBody(pBody); } };
			}

			{
				b2PolygonShape dynamicBox;
				dynamicBox.SetAsBox(5.0f, 5.0f);
				b2FixtureDef fixtureDef;
				fixtureDef.shape = &dynamicBox;
				//fixtureDef.isSensor = isSensor;
				fixtureDef.density = 1.0f;
				fixtureDef.friction = 0.0f;
				fixtureDef.restitution = 1.0f;
				//collision fillter
				//fixtureDef.filter.categoryBits = CollisionFillter::PLAYER;
				//fixtureDef.filter.maskBits = CollisionFillter::ENEMY;
				physic.body->CreateFixture(&fixtureDef);
				physic.body->SetUserEntity(entity);
			}
		}
		
	}
private:
	std::unique_ptr<b2World> box2DEngine;
	entt::DefaultRegistry ECS;
	Codex codex;
	CameraSystem camSystem;
	PlayerControlSystem controlSystem;
	PhysicSystem physicSystem;
	AnimationSystem animSystem;
	DrawSystem drawSystem;
};