#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
struct AnimationComponent
{
	float holdTime;
	float time = 0.0f;
	int iFrame = 0;
	std::vector<sf::IntRect> frames;
};

struct GroupAnimation
{
	int iAnimation = 0;
	std::vector<AnimationComponent> animations;
};