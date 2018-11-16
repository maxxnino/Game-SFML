#pragma once
#include "entt/entt.hpp"
#include "InputSystem.h"
#include "PlayerControlSystem.h"
#include "DrawSystem.h"
#include "Mouse.h"
class World
{
public:
	World()
	{
		if (!texture.loadFromFile("Data\\Images\\enemy_01.png"))
		{
			assert(false);
		}
		texture.setSmooth(true);
	}
	void Update(float dt, Keyboard& kbd, Mouse& mouse)
	{
		input.Update(ECS, kbd);
		controlSystem.Update(ECS, dt);
	}
	void Draw(Graphics& gfx)
	{
		drawSystem.Draw(ECS, gfx, texture);
	}
	void AddPlayer()
	{
		auto entity = ECS.create();
		ECS.assign<InputComponent>(entity);
		ECS.assign<TransformComponent>(entity);
		ECS.assign<VelocityComponent>(entity);
		ECS.assign<MeshComponent>(entity, sf::Vector2f( -50.0f,-50.0f ), sf::Vector2f(50.0f , -50.0f ), sf::Vector2f(50.0f ,50.0f ), sf::Vector2f(-50.0f ,50.0f ));
		ECS.assign<TexCoordsComponent>(entity, sf::Vector2f(0.0f,0.0f ), sf::Vector2f(60.0f,0.0f ), sf::Vector2f(60.0f,60.0f ), sf::Vector2f(0.0f,60.0f ));
	}
private:
	entt::DefaultRegistry ECS;
	sf::Texture texture;
	InputSystem input;
	PlayerControlSystem controlSystem;
	DrawSystem drawSystem;
};