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
			if (Locator::ECS::ref().has<sf::Sprite>() && prevFrame != animation.iCurFrame)
			{
				auto& ECS = Locator::ECS::ref();
				ECS.get<sf::Sprite>(1).setTextureRect(ECS.get<AnimationShareComponent>(animation.shareEntity).frames[animation.iCurFrame]);
			}
		});
	}
};