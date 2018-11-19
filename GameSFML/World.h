#pragma once
#include "entt/entt.hpp"
#include "PlayerControlSystem.h"
#include "DrawSystem.h"
#include "Mouse.h"
#include "TextureManager.h"
#include "PhysicComponent.h"
class World
{
public:
	World()
	{
		box2DEngine = std::make_unique<b2World>(b2Vec2(0.0f, 0.0f));
		textureManager.AddTexture(HashStringManager::Enemy01, "Data\\Images\\enemy_01.png");
		textureManager.AddMesh(HashStringManager::Enemy01, 100.0f, 60.0f, 60.0f);
		textureManager.AddTexture(HashStringManager::Enemy02, "Data\\Images\\enemy_02.png");
		textureManager.AddMesh(HashStringManager::Enemy02, 100.0f, 60.0f, 51.0f);
		textureManager.AddTexture(HashStringManager::Enemy03, "Data\\Images\\enemy_03.png");
		textureManager.AddMesh(HashStringManager::Enemy03, 100.0f, 60.0f, 49.0f);
	}
	void Update(float dt, Keyboard& kbd, Mouse& mouse)
	{
		controlSystem.Update(ECS, dt, kbd, mouse);
	}
	void Draw(Graphics& gfx)
	{
		drawSystem.Draw(ECS, gfx, *box2DEngine,textureManager);
	}
	template <typename entt::HashedString::hash_type meshID>
	void AddPlayerWithTag(const b2Vec2& pos)
	{
		auto entity = ECS.create();
		ECS.assign<entt::label<meshID>>(entity);
		ECS.assign<entt::label<HashStringManager::InputComponent>>(entity);

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
			fixtureDef.filter.categoryBits = CollisionFillter::PLAYER;
			fixtureDef.filter.maskBits = CollisionFillter::ENEMY;
			physic.body->CreateFixture(&fixtureDef);
			physic.body->SetUserEntity(entity);
		}
	}
private:
	std::unique_ptr<b2World> box2DEngine;
	entt::DefaultRegistry ECS;
	TextureManager textureManager;
	PlayerControlSystem controlSystem;
	DrawSystem drawSystem;
};