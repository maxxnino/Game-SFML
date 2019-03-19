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
		Locator::ECS::ref().view<PlayerController, AnimationState>().each([&dir](auto entity, auto&, AnimationState& animState) {
			if (dir == sf::Vector2i(0, 0))
			{
				if (animState == AnimationState::STANDING) return;

				animState = AnimationState::STANDING;
				Locator::ECS::ref().assign<ChangeState>(entity);
				return;
			}

			if (animState == AnimationState::WALKING) return;

			animState = AnimationState::WALKING;
			Locator::ECS::ref().assign<ChangeState>(entity);
		});

		Locator::ECS::ref().view<PlayerController, Direction>().each([&dir](auto entity, auto&, Direction& direction) {
			if (dir.x < 0)
			{
				if (direction == Direction::LEFT) return;

				direction = Direction::LEFT;
				Locator::ECS::ref().assign<ChangeDirection>(entity);
				return;
			}
			
			if (dir.x > 0)
			{
				if (direction == Direction::RIGHT) return;

				direction = Direction::RIGHT;
				Locator::ECS::ref().assign<ChangeDirection>(entity);
				return;
			}

			if (dir.y < 0)
			{
				if (direction == Direction::DOWN) return;

				direction = Direction::DOWN;
				Locator::ECS::ref().assign<ChangeDirection>(entity);
				return;
			}

			if (dir.y > 0)
			{
				if (direction == Direction::UP) return;

				direction = Direction::UP;
				Locator::ECS::ref().assign<ChangeDirection>(entity);
				return;
			}
		});
	}
};
//void aaaa()
//{
//	auto prevDir = state.direction;
//	if (dir.x < 0)
//	{
//		state.direction = AnimationState::Direction::Left;
//	}
//	else
//	{
//		state.direction = AnimationState::Direction::Right;
//	}
//	if (dir.y < 0)
//	{
//		state.direction = AnimationState::Direction::Down;
//	}
//	else
//	{
//		state.direction = AnimationState::Direction::Up;
//	}
//
//	if (prevDir == state.direction) return;
//
//	Locator::ECS::ref().assign<ChangeState>(entity);
//}