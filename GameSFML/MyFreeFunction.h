#pragma once
#include "Locator.h"
#include "Component/PlayerStateComponent.h"
#include "Component/PlayerControllerComponent.h"
#include "Component/AnimationComponent.h"
namespace Maxx
{
	void PlayerUpdateState(uint32_t entity, entt::DefaultRegistry& ECS)
	{
		if (!ECS.has<PlayerStateComponent>(entity) || !ECS.has<PlayerControllerComponent>(entity)) return;

		auto& state = ECS.get<PlayerStateComponent>(entity);
		auto& controller = ECS.get<PlayerControllerComponent>(entity);
		if (controller.direction == sf::Vector2i(0, 0))
		{
			if (state.state == PlayerStateComponent::State::Standing) return;
			
			state.state = PlayerStateComponent::State::Standing;
			
			if (!ECS.has<AnimationComponent>(entity)) return;

			auto& animation = ECS.get<AnimationComponent>(entity);
			animation.curFrameTime = 0.0f;
			animation.iCurFrame = animation.rangeIndex.first;
			animation.rangeIndex.second = animation.rangeIndex.first;
		}
		else
		{
			if (controller.direction.x < 0)
			{
				if (state.direction == PlayerStateComponent::Direction::Left) return;
				
				state.direction = PlayerStateComponent::Direction::Left;

				if (!ECS.has<AnimationComponent>(entity)) return;
				auto& animation = ECS.get<AnimationComponent>(entity);

				animation.curFrameTime = 0.0f;
				animation.rangeIndex = animation.resource->animationSets[static_cast<unsigned int>(state.direction)];
				animation.iCurFrame = animation.rangeIndex.first;
				return;
			}
				
			if (controller.direction.x > 0)
			{
				if (state.direction == PlayerStateComponent::Direction::Right) return;
				
				state.direction = PlayerStateComponent::Direction::Right;
				if (!ECS.has<AnimationComponent>(entity)) return;
				auto& animation = ECS.get<AnimationComponent>(entity);

				animation.curFrameTime = 0.0f;
				animation.rangeIndex = animation.resource->animationSets[static_cast<unsigned int>(state.direction)];
				animation.iCurFrame = animation.rangeIndex.first;
				return;
			}
				
			if (controller.direction.y < 0)
			{
				if (state.direction == PlayerStateComponent::Direction::Down) return;
				
				state.direction = PlayerStateComponent::Direction::Down;
				if (!ECS.has<AnimationComponent>(entity)) return;
				auto& animation = ECS.get<AnimationComponent>(entity);

				animation.curFrameTime = 0.0f;
				animation.rangeIndex = animation.resource->animationSets[static_cast<unsigned int>(state.direction)];
				animation.iCurFrame = animation.rangeIndex.first;
				return;
			}
				
			if (controller.direction.y > 0)
			{
				if (state.direction == PlayerStateComponent::Direction::Up) return;
				
				state.direction = PlayerStateComponent::Direction::Up;
				if (!ECS.has<AnimationComponent>(entity)) return;
				auto& animation = ECS.get<AnimationComponent>(entity);

				animation.curFrameTime = 0.0f;
				animation.rangeIndex = animation.resource->animationSets[static_cast<unsigned int>(state.direction)];
				animation.iCurFrame = animation.rangeIndex.first;
				return;
			}
		}
	}
}