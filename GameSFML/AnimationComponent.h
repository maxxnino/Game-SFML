#pragma once
#include "SFML/Graphics.hpp"
struct AnimationComponent
{
	float holdTime;
	float curFrameTime = 0.0f;
	uint32_t iCurFrame = 0;
	uint32_t maxFrame;
	uint32_t shareEntity;
};
struct AnimationShareComponent
{
	std::vector<sf::IntRect> frames;
};
struct UpdateAnimation
{
	uint32_t shareEntity;
	uint32_t iCurFrame;
};