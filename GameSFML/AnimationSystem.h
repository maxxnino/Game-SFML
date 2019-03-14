#pragma once
#include "Locator.h"
#include "AnimationComponent.h"
class AnimationSystem
{
public:
	void Update(float dt)
	{
		Locator::ECS::ref().reset<UpdateAnimation>();
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
			if (prevFrame != animation.iCurFrame && Locator::ECS::ref().has<AnimationShareComponent>(animation.shareEntity))
			{
				Locator::ECS::ref().assign<UpdateAnimation>(entity, animation.shareEntity, animation.iCurFrame);
			}
		});
	}
	void WarmUp()
	{
		Locator::ECS::ref().view<UpdateAnimation, sf::Sprite>().each([](auto entity, UpdateAnimation &updateAnimation, sf::Sprite &sprite) {
			sprite.setTextureRect(Locator::ECS::ref().get<AnimationShareComponent>(
				updateAnimation.shareEntity).frames[updateAnimation.iCurFrame]
			);
		});
	}
};