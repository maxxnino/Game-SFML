#pragma once
#include "entt/entt.hpp"
#include "InputSystem.h"
#include "PlayerControlSystem.h"
#include "DrawSystem.h"
#include "Mouse.h"
#include "TextureManager.h"
#include "HashStringManager.h"
class World
{
public:
	World()
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
		input.Update(ECS, kbd);
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
	}
private:
	entt::DefaultRegistry ECS;
	TextureManager texManager;
	InputSystem input;
	PlayerControlSystem controlSystem;
	DrawSystem drawSystem;
};