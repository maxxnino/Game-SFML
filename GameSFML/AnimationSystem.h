#pragma once
#include "Locator.h"
#include "AnimationComponent.h"
class AnimationSystem
{
public:
	void Update(float dt)
	{
		Locator::ECS::ref().view<AnimationComponent>().each([dt](auto entity, AnimationComponent& animation) {
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
	}
};