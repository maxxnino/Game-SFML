#pragma once
#include "System/ISystemECS.h"
#include "Component/AnimationComponent.h"
class AnimationSystem : public ISystemECS
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		auto view = ECS.view<AnimationComponent>();
		std::for_each(std::execution::par, view.begin(), view.end(), [&ECS, dt](auto entity) {
			auto& animation = ECS.get<AnimationComponent>(entity);

			animation.curFrameTime += dt;
			//const auto prevFrame = animation.iCurFrame;
			while (animation.curFrameTime >= animation.holdTime)
			{
				animation.curFrameTime -= animation.holdTime;
				animation.iCurFrame++;
				if (animation.iCurFrame >= animation.rangeIndex.second)
				{
					animation.iCurFrame = animation.rangeIndex.first;
				}
			}
		});
	}
};
/*
void BackUp()
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
}
*/