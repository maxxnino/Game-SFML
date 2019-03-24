#pragma once
#include "SFML/Graphics.hpp"
struct StaticObjectSpawnInfo
{
	float height, width;
	sf::Vector2f pos;
	unsigned int rotation;
};

struct StaticCustomObjectSpawnInfo
{
	float height, width;
	sf::Vector2f pos;
	unsigned int rotation;
};