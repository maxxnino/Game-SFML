#pragma once
#include "SFML/Graphics/Texture.hpp"
#include "entt/core/hashed_string.hpp"
#include <unordered_map>
struct AnimationResource
{
	std::vector<sf::IntRect> frames;
	const sf::Texture* texture = nullptr;
	unsigned int width, height, tileWidth, tileHeight, frameTime;
	//hold begin index and end index in farmes, each pair is 1 animation set
	std::vector<std::pair<unsigned int, unsigned int>> animationSets;
};


struct MapResource
{
	struct Object
	{
		float height, width, x, y;
		unsigned int rotation;
	};
	int tileSize = 0, gridW = 0, gridH = 0;
	const sf::Texture* tileTexture = nullptr;
	//layers tile
	std::vector<std::vector<unsigned int>> layers;

	//layer with tile has object
	std::vector<unsigned int> tilesWithObject;

	//object attached per tile
	std::unordered_map<unsigned int, Object> objects;

	//custom object, will add more in the future with sensor or static objects
	std::vector<Object> objectLayer;
};
