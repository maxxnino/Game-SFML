#pragma once
#include "Locator.h"
#include "GameplayTags.h"
#include "AnimationComponent.h"
class PlayerControllerSystem
{
public:
	void Update()
	{
		auto& kbd = Locator::Keyboard::ref();
		sf::Vector2i dir{ 0,0 };
		if (kbd.KeyIsPressed(sf::Keyboard::A))
		{
			dir.x = -1;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::D))
		{
			dir.x = 1;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::S))
		{
			dir.y = -1;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::W))
		{
			dir.y = 1;
		}
		Locator::ECS::ref().view<PlayerController>().each([&dir](auto entity, auto&) {
			auto prevState = state.state;
			if (dir == sf::Vector2i(0, 0))
			{
				state.state = AnimationState::State::Standing;
				if (state.state == prevState) return;

				Locator::ECS::ref().assign<ChangeState>(entity);
				return;
			}
			state.state = AnimationState::State::Walking;

			auto prevDir = state.direction;
			if (dir.x < 0)
			{
				state.direction = AnimationState::Direction::Left;
			}
			else
			{
				state.direction = AnimationState::Direction::Right;
			}
			if (dir.y < 0)
			{
				state.direction = AnimationState::Direction::Down;
			}
			else
			{
				state.direction = AnimationState::Direction::Up;
			}

			if (prevDir == state.direction) return;

			Locator::ECS::ref().assign<ChangeState>(entity);
		});
	}
};