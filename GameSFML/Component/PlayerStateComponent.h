#pragma once
#include "../Component/PlayerControllerComponent.h"
#include "entt/entt.hpp"
#include "../Component/AnimationComponent.h"
struct PlayerStateComponent
{
	enum State
	{
		Standing,
		Walking
	} state = State::Standing;
	enum Direction
	{
		Up = 0,
		Left = 1,
		Down = 2,
		Right = 3
	} direction = Direction::Left;
};
struct UpdateState
{
	static void Player(uint32_t entity, entt::DefaultRegistry& ECS)
	{
		if (!ECS.has<PlayerStateComponent>(entity) || !ECS.has<PlayerControllerComponent>(entity)) return;

		auto& state = ECS.get<PlayerStateComponent>(entity);
		auto& controller = ECS.get<PlayerControllerComponent>(entity);

		if (state.state == PlayerStateComponent::State::Standing)
		{
			if (controller.direction == sf::Vector2i(0, 0)) return;

			state.state = PlayerStateComponent::State::Walking;

			if (controller.direction.x < 0)
			{
				state.direction = PlayerStateComponent::Direction::Left;
				if (ECS.has<AnimationComponent>(entity))
				{
					auto& animation = ECS.get<AnimationComponent>(entity);

					animation.curFrameTime = 0.0f;
					animation.rangeIndex = animation.resource->animationSets[static_cast<unsigned int>(state.direction)];
					animation.iCurFrame = animation.rangeIndex.first + 1;
				}
			}

			if (controller.direction.x > 0)
			{
				state.direction = PlayerStateComponent::Direction::Right;
				if (ECS.has<AnimationComponent>(entity))
				{
					auto& animation = ECS.get<AnimationComponent>(entity);
					animation.curFrameTime = 0.0f;
					animation.rangeIndex = animation.resource->animationSets[static_cast<unsigned int>(state.direction)];
					animation.iCurFrame = animation.rangeIndex.first + 1;
				}
			}

			if (controller.direction.y < 0)
			{
				state.direction = PlayerStateComponent::Direction::Down;
				if (ECS.has<AnimationComponent>(entity))
				{
					auto& animation = ECS.get<AnimationComponent>(entity);
					animation.curFrameTime = 0.0f;
					animation.rangeIndex = animation.resource->animationSets[static_cast<unsigned int>(state.direction)];
					animation.iCurFrame = animation.rangeIndex.first + 1;
				}
			}

			if (controller.direction.y > 0)
			{
				state.direction = PlayerStateComponent::Direction::Up;
				if (ECS.has<AnimationComponent>(entity))
				{
					auto& animation = ECS.get<AnimationComponent>(entity);
					animation.curFrameTime = 0.0f;
					animation.rangeIndex = animation.resource->animationSets[static_cast<unsigned int>(state.direction)];
					animation.iCurFrame = animation.rangeIndex.first + 1;
				}
			}
		}
		else
		{
			if (controller.direction == sf::Vector2i(0, 0))
			{
				state.state = PlayerStateComponent::State::Standing;
				if (ECS.has<AnimationComponent>(entity))
				{
					auto& animation = ECS.get<AnimationComponent>(entity);
					animation.curFrameTime = 0.0f;
					animation.iCurFrame = animation.rangeIndex.first;
					animation.rangeIndex.second = animation.rangeIndex.first;
				}
			}
			else
			{
				state.state = PlayerStateComponent::State::Walking;

				if (controller.direction.x < 0)
				{
					if (state.direction != PlayerStateComponent::Direction::Left)
					{
						state.direction = PlayerStateComponent::Direction::Left;
						if (ECS.has<AnimationComponent>(entity))
						{
							auto& animation = ECS.get<AnimationComponent>(entity);

							animation.curFrameTime = 0.0f;
							animation.rangeIndex = animation.resource->animationSets[static_cast<unsigned int>(state.direction)];
							animation.iCurFrame = animation.rangeIndex.first + 1;
						}
					}
				}

				if (controller.direction.x > 0)
				{
					if (state.direction != PlayerStateComponent::Direction::Right)
					{
						state.direction = PlayerStateComponent::Direction::Right;
						if (ECS.has<AnimationComponent>(entity))
						{
							auto& animation = ECS.get<AnimationComponent>(entity);
							animation.curFrameTime = 0.0f;
							animation.rangeIndex = animation.resource->animationSets[static_cast<unsigned int>(state.direction)];
							animation.iCurFrame = animation.rangeIndex.first + 1;
						}
					}
				}

				if (controller.direction.y < 0)
				{
					if (state.direction != PlayerStateComponent::Direction::Down)
					{
						state.direction = PlayerStateComponent::Direction::Down;
						if (ECS.has<AnimationComponent>(entity))
						{
							auto& animation = ECS.get<AnimationComponent>(entity);
							animation.curFrameTime = 0.0f;
							animation.rangeIndex = animation.resource->animationSets[static_cast<unsigned int>(state.direction)];
							animation.iCurFrame = animation.rangeIndex.first + 1;
						}
					}
				}

				if (controller.direction.y > 0)
				{
					if (state.direction != PlayerStateComponent::Direction::Up)
					{
						state.direction = PlayerStateComponent::Direction::Up;
						if (ECS.has<AnimationComponent>(entity))
						{
							auto& animation = ECS.get<AnimationComponent>(entity);
							animation.curFrameTime = 0.0f;
							animation.rangeIndex = animation.resource->animationSets[static_cast<unsigned int>(state.direction)];
							animation.iCurFrame = animation.rangeIndex.first + 1;
						}
					}
				}
			}
		}
	}
};