#pragma once
#include <assert.h>
#include <iostream>
#include "SFML/Graphics.hpp"
#include "GameResource.h"
#include "HashStringDataBase.h"
#include "Component/StaticObjectSpawnInfo.h"
#include "Component/GameplayTags.h"
//moving grid not working yet
class Grid : public sf::Drawable, public sf::Transformable
{
public:
	template<typename Loader>
	void LoadFromFile(Loader& loader, entt::DefaultRegistry& ECS)
	{
		const auto& resource = loader.GetMapResource(Database::GridMap);
		if (resource.tileSize <= 0 || resource.gridW <= 0 || resource.gridH <= 0 || resource.tileTexture == nullptr)
		{
			assert(false);
			return;
		}
		//grid
		tileSize = resource.tileSize;
		gridW = resource.gridW;
		gridH = resource.gridH;

		//texture
		tileTexture = resource.tileTexture;
		const int textureTileW = (int)tileTexture->getSize().x / tileSize;
		const int textureTileH = (int)tileTexture->getSize().y / tileSize;

		layers = &resource.layers;
		drawLayers.setPrimitiveType(sf::Quads);

		//make vertex form layer map
		for (auto& layer : resource.layers)
		{
			staticLayers.emplace_back(sf::VertexArray(sf::Quads, gridW * gridH * 4));
			

			for (int i = 0; i < gridW; ++i)
			{
				for (int j = 0; j < gridH; ++j)
				{
					//get texture position
					const unsigned int textureLookup = layer[i + j * gridW];
					if (textureLookup == 0) continue;
					
					const int textureX = (textureLookup % textureTileW) == 0 ? textureTileW : textureLookup % textureTileW;
					const int textureY = textureLookup / textureTileW;
					// get a pointer to the current tile's quad
					sf::Vertex* quad = &staticLayers.back()[(i + j * gridW) * 4];

					// define its 4 corners
					quad[0].position = sf::Vector2f(float(i * tileSize), float(j * tileSize));
					quad[1].position = sf::Vector2f(float((i + 1) * tileSize), float(j * tileSize));
					quad[2].position = sf::Vector2f(float((i + 1) * tileSize), float((j + 1) * tileSize));
					quad[3].position = sf::Vector2f(float(i * tileSize), float((j + 1) * tileSize));

					// define its 4 texture coordinates
					quad[0].texCoords = sf::Vector2f(float((textureX - 1) * tileSize), float(textureY * tileSize));
					quad[1].texCoords = sf::Vector2f(float(textureX * tileSize), float(textureY * tileSize));
					quad[2].texCoords = sf::Vector2f(float(textureX * tileSize), float((textureY + 1) * tileSize));
					quad[3].texCoords = sf::Vector2f(float((textureX - 1) * tileSize), float((textureY + 1) * tileSize));
				}
			}
				
		}

		//make vertex form tileObject
		{
			staticLayers.emplace_back(sf::VertexArray(sf::Quads, gridW * gridH * 4));
			tileObjectLayer = &resource.tilesWithObject;
			for (int i = 0; i < gridW; ++i)
			{
				for (int j = 0; j < gridH; ++j)
				{
					//get texture position
					const unsigned int textureLookup = resource.tilesWithObject[i + j * gridW];
					if (textureLookup == 0) continue;

					const int textureX = (textureLookup % textureTileW) == 0 ? textureTileW : textureLookup % textureTileW;
					const int textureY = textureLookup / textureTileW;
					// get a pointer to the current tile's quad
					sf::Vertex* quad = &staticLayers.back()[(i + j * gridW) * 4];

					// define its 4 corners
					quad[0].position = sf::Vector2f(float(i * tileSize), float(j * tileSize));
					quad[1].position = sf::Vector2f(float((i + 1) * tileSize), float(j * tileSize));
					quad[2].position = sf::Vector2f(float((i + 1) * tileSize), float((j + 1) * tileSize));
					quad[3].position = sf::Vector2f(float(i * tileSize), float((j + 1) * tileSize));

					// define its 4 texture coordinates
					quad[0].texCoords = sf::Vector2f(float((textureX - 1) * tileSize), float(textureY * tileSize));
					quad[1].texCoords = sf::Vector2f(float(textureX * tileSize), float(textureY * tileSize));
					quad[2].texCoords = sf::Vector2f(float(textureX * tileSize), float((textureY + 1) * tileSize));
					quad[3].texCoords = sf::Vector2f(float((textureX - 1) * tileSize), float((textureY + 1) * tileSize));

					//find ofject location
					auto object = resource.objects.find(textureLookup);
					if (object == resource.objects.end()) continue;

					auto entity = ECS.create();
					auto& spawnInfo = ECS.assign<StaticObjectSpawnInfo>(entity);
					spawnInfo.height = object->second.height * 0.5f;
					spawnInfo.width = object->second.width * 0.5f;
					spawnInfo.pos = quad[0].position + sf::Vector2f(object->second.x + spawnInfo.width, object->second.y + spawnInfo.height);
					spawnInfo.rotation = object->second.rotation;
				}
			}
		}
		//make Object From Layer Object
		for (auto& object : resource.objectLayer)
		{
			auto entity = ECS.create();
			auto& spawnInfo = ECS.assign<StaticCustomObjectSpawnInfo>(entity);
			spawnInfo.height = object.height * 0.5f;
			spawnInfo.width = object.width * 0.5f;
			spawnInfo.pos = {object.x + spawnInfo.width, object.y + spawnInfo.height };
			spawnInfo.rotation = object.rotation;
		}

	}
	void Culling(sf::Vector2f topLeft, sf::Vector2f rightBottom)
	{
		int left = 0, right = 0, top = 0, bottom = 0;
		//get top left point of view
		//topLeft -= getPosition();
		left = int(topLeft.x / tileSize);
		top = int(topLeft.y / tileSize);

		//get bottom right point of view
		//rightBottom -= getPosition();
		right = 1 + int(rightBottom.x / tileSize);
		bottom = 1 + int(rightBottom.y / tileSize);

		//clamp these to fit into array bounds:
		left = std::max(0, std::min(left, gridW));
		top = std::max(0, std::min(top, gridH));
		right = std::max(0, std::min(right, gridW));
		bottom = std::max(0, std::min(bottom, gridH));

		//build visible vertercies:
		drawLayers.clear();
		for (int nl = 0; nl < staticLayers.size() - 1; nl++)
		{
			for (int i = left; i < right; ++i)
			{
				for (int j = top; j < bottom; ++j)
				{
					//skip if current tile in texture map is 0
					if ((*layers)[nl][i + j * gridW] == 0) continue;

					const int cur = (i + j * gridW) * 4;

					// define its 4 corners
					drawLayers.append(staticLayers[nl][cur]);
					drawLayers.append(staticLayers[nl][cur + 1]);
					drawLayers.append(staticLayers[nl][cur + 2]);
					drawLayers.append(staticLayers[nl][cur + 3]);
				}
			}
		}
		
		for (int i = left; i < right; ++i)
		{
			for (int j = top; j < bottom; ++j)
			{
				//skip if current tile in texture map is 0
				if ((*tileObjectLayer)[i + j * gridW] == 0) continue;

				const int cur = (i + j * gridW) * 4;

				// define its 4 corners
				drawLayers.append(staticLayers.back()[cur]);
				drawLayers.append(staticLayers.back()[cur + 1]);
				drawLayers.append(staticLayers.back()[cur + 2]);
				drawLayers.append(staticLayers.back()[cur + 3]);
			}
		}
		//std::cout << "Vertices: " << drawLayers.getVertexCount() << ", Tiles: " << drawLayers.getVertexCount() / 4 << std::endl;
	}
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const final
	{
		if (tileSize <= 0 || gridW <= 0 || gridH <= 0 || tileTexture == nullptr)
		{
			assert(false);
			return;
		}
		states.texture = tileTexture;
		//states.transform *= getTransform();
		target.draw(drawLayers, states);	
	}
private:
	int tileSize = 40, gridW = 0, gridH = 0;
	const sf::Texture* tileTexture = nullptr;
	const std::vector<std::vector<unsigned int>>* layers;
	const std::vector<unsigned int>* tileObjectLayer;
	std::vector<sf::VertexArray> staticLayers;
	sf::VertexArray drawLayers;
	std::vector<MapResource::Object> staticObjects;
};