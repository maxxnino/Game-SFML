#pragma once
#include "SFML/Graphics.hpp"
struct ModelComponent
{
	sf::Vector2f meshP01{ 0.0f,0.0f };
	sf::Vector2f meshP02{ 0.0f,0.0f };
	sf::Vector2f meshP03{ 0.0f,0.0f };
	sf::Vector2f meshP04{ 0.0f,0.0f };

	sf::Vector2f texCoord01{ 0.0f,0.0f };
	sf::Vector2f texCoord02{ 0.0f,0.0f };
	sf::Vector2f texCoord03{ 0.0f,0.0f };
	sf::Vector2f texCoord04{ 0.0f,0.0f };
};