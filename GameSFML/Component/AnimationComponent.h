#pragma once
#include "SFML/Graphics.hpp"
#include "GameResource.h"
#include <vector>
struct AnimationComponent
{
	AnimationComponent(const AnimationResource& resource, unsigned int iAnimationSet)
		:
		holdTime((float)resource.frameTime / 60.0f),
		rangeIndex(resource.animationSets[iAnimationSet]),
		frames(&resource.frames)
	{}
	float holdTime;
	float curFrameTime = 0.0f;
	unsigned int iCurFrame = 0;
	std::pair<unsigned int, unsigned int> rangeIndex;
	const std::vector<sf::IntRect>* frames = nullptr;
};