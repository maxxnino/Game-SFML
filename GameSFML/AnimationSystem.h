#pragma once
#include "entt/entt.hpp"
#include "AnimationComponent.h"
class AnimationSystem
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt)
	{
		ECS.view<GroupAnimation, sf::Sprite>().each([&](const auto, GroupAnimation& animation, sf::Sprite& sprite) {
			AnimationComponent& curAnimation = animation.animations[animation.iAnimation];

			curAnimation.time += dt;
			while (curAnimation.time >= curAnimation.holdTime)
			{
				curAnimation.time -= curAnimation.holdTime;
				if (++curAnimation.iFrame >= curAnimation.frames.size())
				{
					curAnimation.iFrame = 0;
				}
			}
			sprite.setTextureRect(curAnimation.frames[curAnimation.iFrame]);
		});
	}
};