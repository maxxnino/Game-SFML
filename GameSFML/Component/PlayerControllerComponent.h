#pragma once
#include "SFML/System/Vector2.hpp"
struct PlayerControllerComponent
{
	sf::Vector2i direction;
	bool bIsShooting = false;
	sf::Vector2i mousePos;
};