#pragma once
#include "entt/signal/sigh.hpp"
struct AnimationStateComponent 
{
	enum AnimationState
	{
		WALKING,
		STANDING
	} state = AnimationState::STANDING;
	enum Direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT
	} direction = Direction::DOWN;
	entt::SigH<void(AnimationStateComponent)> changeTransitionRule;
};