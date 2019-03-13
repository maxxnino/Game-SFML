#pragma once
#include "SFML/Graphics.hpp"
struct AnimationComponent
{
	uint32_t shareEntity;
	uint32_t iCurFrame = 0;
	float curFrameTime = 0.0f;
	float holdTime;
	uint32_t maxFrame;
};
struct AnimationShareComponent
{
	std::vector<sf::IntRect> frames;
};