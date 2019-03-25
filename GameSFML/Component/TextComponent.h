#pragma once
#include "SFML/Graphics.hpp"

struct UpdateTextFont
{
	const sf::Font* newFont = nullptr;
};
struct UpdateTextString
{
	std::string newString;
};
struct UpdateTextLocation
{
	sf::Vector2f newLocation;
};
struct TextLocation
{
	//this is the location offset from central of the screen
	sf::Vector2f location;
};