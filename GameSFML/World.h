#pragma once
#include "entt/entt.hpp"
#include "InputSystem.h"
#include "PlayerControlSystem.h"
#include "DrawSystem.h"
#include "Mouse.h"
#include "TextureManager.h"

class World
{
public:
	World()
	{
		texManager.AddTexture("Enemy01", "Data\\Images\\enemy_01.png");
		texManager.AddMesh("Enemy01", 100.0f);
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
	void AddPlayer(sf::Vector2f pos)
	{
		auto entity = ECS.create();
		ECS.assign<InputComponent>(entity);
		ECS.assign<TransformComponent>(entity).position = pos;
		
		ECS.assign<VelocityComponent>(entity);
		ECS.assign<TexCoordsComponent>(entity, sf::Vector2f(0.0f,0.0f ), sf::Vector2f(60.0f,0.0f ), sf::Vector2f(60.0f,60.0f ), sf::Vector2f(0.0f,60.0f ));
		ECS.assign<entt::label<"Enemy01"_hs>>(entity);
	}
private:
	entt::DefaultRegistry ECS;
	TextureManager texManager;
	InputSystem input;
	PlayerControlSystem controlSystem;
	DrawSystem drawSystem;
};