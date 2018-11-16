#pragma once
#include "SFML/Graphics.hpp"
struct TransformComponent
{
	sf::Vector2f position = { 0.0f,0.0f };
	float rotation = 0.0f;
};