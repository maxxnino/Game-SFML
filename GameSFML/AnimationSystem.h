#pragma once
#include "ISystem.h"
#include "Locator.h"
#include "AnimationComponent.h"
class AnimationSystem : public ISystem
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		ECS.view<ChangeDirection, Direction, AnimationComponent>().each([](auto entity, auto&, Direction& direction, AnimationComponent& animation) {
			switch (direction)
			{
			case Direction::DOWN:
				animation.frames = &Locator::Codex::ref().GetFramesRect("PlayerDown"_hs).frames;
				break;
			case Direction::UP:
				animation.frames = &Locator::Codex::ref().GetFramesRect("PlayerUp"_hs).frames;
				break;
			case Direction::LEFT:
				animation.frames = &Locator::Codex::ref().GetFramesRect("PlayerLeft"_hs).frames;
				break;
			case Direction::RIGHT:
				animation.frames = &Locator::Codex::ref().GetFramesRect("PlayerRight"_hs).frames;
				break;
			default:
				break;
			}
		});
		ECS.view<AnimationComponent>().each([dt](auto entity, AnimationComponent& animation) {
			animation.curFrameTime += dt;
			const auto prevFrame = animation.iCurFrame;
			while (animation.curFrameTime >= animation.holdTime)
			{
				animation.curFrameTime -= animation.holdTime;
				animation.iCurFrame++;
				if (animation.iCurFrame >= animation.maxFrame)
				{
					animation.iCurFrame = 0;
				}
			}
		});
		ECS.reset<ChangeDirection>();
		ECS.reset<ChangeState>();
	}
};