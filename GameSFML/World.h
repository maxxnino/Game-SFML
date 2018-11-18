#pragma once
#include "entt/entt.hpp"
#include "InputSystem.h"
#include "PlayerControlSystem.h"
#include "DrawSystem.h"
#include "Mouse.h"
#include "TextureManager.h"
#include "PhysicComponent.h"
class World
{
public:
	World()
		:
		drawSystem(ECS)
	{
		texManager.AddTexture(HashStringManager::Enemy01, "Data\\Images\\enemy_01.png");
		texManager.AddMesh(HashStringManager::Enemy01, 100.0f, 60.0f, 60.0f);
		texManager.AddTexture(HashStringManager::Enemy02, "Data\\Images\\enemy_02.png");
		texManager.AddMesh(HashStringManager::Enemy02, 100.0f, 60.0f, 51.0f);
		texManager.AddTexture(HashStringManager::Enemy03, "Data\\Images\\enemy_03.png");
		texManager.AddMesh(HashStringManager::Enemy03, 100.0f, 60.0f, 49.0f);
	}
	void Update(float dt, Keyboard& kbd, Mouse& mouse)
	{
		//input.Update(ECS, kbd);
		controlSystem.Update(ECS, dt);
	}
	void Draw(Graphics& gfx)
	{
		drawSystem.Draw(ECS, gfx, texManager);
	}
	template <typename entt::HashedString::hash_type value>
	void AddPlayerWithTag(sf::Vector2f pos)
	{
		auto entity = ECS.create();
		ECS.assign<InputComponent>(entity);
		ECS.assign<TransformComponent>(entity).position = pos;
		
		ECS.assign<VelocityComponent>(entity);
		ECS.assign<entt::label<value>>(entity);

		//auto& physic = ECSManager.assign<PhysicComponent>(entity);
		//{
		//	b2BodyDef bodyDef;
		//	bodyDef.type = b2_dynamicBody;
		//	bodyDef.fixedRotation = true;
		//	bodyDef.position.Set(pos.x, pos.y);
		//	physic.body = { box2DEngine->CreateBody(&bodyDef),[this](b2Body* pBody) {box2DEngine->DestroyBody(pBody); } };
		//}

		//{
		//	b2PolygonShape dynamicBox;
		//	dynamicBox.SetAsBox(1.0f, 1.0f);
		//	b2FixtureDef fixtureDef;
		//	fixtureDef.shape = &dynamicBox;
		//	//fixtureDef.isSensor = isSensor;
		//	fixtureDef.density = 1.0f;
		//	fixtureDef.friction = 0.0f;
		//	fixtureDef.restitution = 1.0f;
		//	//collision fillter
		//	fixtureDef.filter.categoryBits = CollisionFillter::PLAYER;
		//	fixtureDef.filter.maskBits = CollisionFillter::ENEMY;
		//	physic.body->CreateFixture(&fixtureDef);
		//	physic.body->SetUserEntity(entity);
		//}
	}
private:
	entt::DefaultRegistry ECS;
	TextureManager texManager;
	InputSystem input;
	PlayerControlSystem controlSystem;
	DrawSystem drawSystem;
};