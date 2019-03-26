#pragma once
#include "SFML/Graphics.hpp"
#include "GameResource.h"
#include <vector>
struct AnimationComponent
{
	AnimationComponent(const AnimationResource& resource, unsigned int iAnimationSet = 0)
		:
		holdTime((float)resource.frameTime / 1000.0f),
		rangeIndex(resource.animationSets[iAnimationSet]),
		frames(&resource.frames),
		resource(&resource)
	{
		rangeIndex.second = rangeIndex.first;
	}
	float holdTime;
	float curFrameTime = 0.0f;
	unsigned char iCurFrame = 0;
	std::pair<unsigned char, unsigned char> rangeIndex;
	//will use share component in the next entt update
	const std::vector<sf::IntRect>* frames = nullptr;
	const AnimationResource* resource = nullptr;
};