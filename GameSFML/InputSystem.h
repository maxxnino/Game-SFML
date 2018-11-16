#pragma once
#include "Keyboard.h"
#include "entt/entt.hpp"
#include "InputComponent.h"
class InputSystem
{
public:
	void Update(entt::DefaultRegistry& ECS, Keyboard& kbd)
	{
		ECS.view<InputComponent>().each([&](auto & entity, InputComponent& input) {
			input.dir.x = 0.0f;
			input.dir.y = 0.0f;
			input.rot = 0.0f;
			if (kbd.KeyIsPressed(sf::Keyboard::A))
			{
				input.dir.x = -1.0f;
			}
			if (kbd.KeyIsPressed(sf::Keyboard::D))
			{
				input.dir.x = 1.0f;
			}
			if (kbd.KeyIsPressed(sf::Keyboard::S))
			{
				input.dir.y = 1.0f;
			}
			if (kbd.KeyIsPressed(sf::Keyboard::W))
			{
				input.dir.y = -1.0f;
			}
			if (kbd.KeyIsPressed(sf::Keyboard::Q))
			{
				input.rot = 1.0f;
			}
			if (kbd.KeyIsPressed(sf::Keyboard::E))
			{
				input.rot = -1.0f;
			}
		});	
	}
};