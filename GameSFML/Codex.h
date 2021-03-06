#pragma once

#include "entt/resource/cache.hpp"
#include "GameResource.h"
#include "json.hpp"
#include <fstream>
#include <sstream>
struct TextureLoader final : entt::ResourceLoader<TextureLoader, sf::Texture> {
	std::shared_ptr<sf::Texture> load(entt::HashedString filename) const
	{
		auto resource = std::make_shared<sf::Texture>();

		if (!resource->loadFromFile(static_cast<const char *>(filename)))
		{
			assert(false);
		}
		
		return resource;
	}
};
struct AnimationLoader final : entt::ResourceLoader<AnimationLoader, AnimationResource> {
	std::shared_ptr<AnimationResource> load(const nlohmann::json& Json, const sf::Texture& texture) const
	{
		auto resource = std::make_shared<AnimationResource>();
		resource->texture = &texture;
		resource->width = Json["width"].get<unsigned int>();
		resource->height = Json["height"].get<unsigned int>();
		resource->tileWidth = Json["tileWidth"].get<unsigned int>();
		resource->tileHeight = Json["tileHeight"].get<unsigned int>();
		resource->frameTime = Json["frameTime"].get<unsigned int>();

		const unsigned int frameX = resource->width / resource->tileWidth;
		const unsigned int frameY = resource->height / resource->tileHeight;
		for (unsigned int y = 0; y < frameY; y++)
		{
			for (unsigned int x = 0; x < frameX; x++)
			{
				resource->frames.emplace_back(sf::IntRect(x * resource->tileWidth, y * resource->tileHeight, resource->tileWidth, resource->tileHeight));
			}
		}
		
		for (auto& set : Json["animationSets"])
		{
			resource->animationSets.emplace_back(set["index"].get<std::pair<unsigned char, unsigned char>>());
		}
		return resource;
	}
};
struct MapLoader final : entt::ResourceLoader<MapLoader, MapResource> {
	std::shared_ptr<MapResource> load(const nlohmann::json& Json, const sf::Texture& texture) const
	{
		auto resource = std::make_shared<MapResource>();

		//load texture
		resource->tileSize = Json["tilewidth"].get<int>();
		resource->gridW = Json["width"].get<int>();
		resource->gridH = Json["height"].get<int>();
		resource->tileTexture = &texture;

		//load layer map
		for (auto& layer : Json["layers"])
		{
			//add layer
			if (layer.find("data") != layer.end())
			{
				if (layer["name"].get<std::string>() == "TileObject")
				{
					resource->tilesWithObject = layer["data"].get<std::vector<unsigned int>>();
				}
				else
				{
					resource->layers.emplace_back(layer["data"].get<std::vector<unsigned int>>());
				}
				
				continue;
			}
			// add object layer
			if (layer.find("objects") != layer.end())
			{
				for (auto& object : layer["objects"])
				{
					resource->objectLayer.emplace_back(MapResource::Object{
						object["height"].get<float>(),
						object["width"].get<float>(),
						object["x"].get<float>(),
						object["y"].get<float>(),
						object["rotation"].get<unsigned int>() 
					});
				}
			}
		}

		//load ofject per tile
		for (auto& tileset : Json["tilesets"])
		{
			for (auto& tile : tileset["tiles"])
			{
				if (tile.find("objectgroup") != tile.end())
				{
					const unsigned int id = tile["id"].get<unsigned int>() + 1u;
					for (auto& object : tile["objectgroup"]["objects"])
					{
						resource->objects.emplace(id, MapResource::Object{
							object["height"].get<float>(),
							object["width"].get<float>(),
							object["x"].get<float>(),
							object["y"].get<float>(),
							object["rotation"].get<unsigned int>()
						});
					}
				}
			}
		}


		return resource;
	}
};
struct FontLoader final : entt::ResourceLoader<FontLoader, sf::Font> {
	std::shared_ptr<sf::Font> load(entt::HashedString filename) const
	{
		auto resource = std::make_shared<sf::Font>();

		if (!resource->loadFromFile(static_cast<const char *>(filename)))
		{
			assert(false);
		}

		return resource;
	}
};
class Codex
{
public:
	const sf::Texture& GetTexture(entt::HashedString filename)
	{
		if (textureCache.contains(filename))
		{
			return textureCache.handle(filename).get();
		}

		textureCache.load<TextureLoader>(filename, filename);
		return textureCache.handle(filename).get();
	}
	const AnimationResource& GetAnimation(entt::HashedString filename)
	{
		if (animationCache.contains(filename))
		{
			return animationCache.handle(filename).get();
		}
		const nlohmann::json Json = GetJson(filename);

		std::string imageName{ "Data\\Images\\" };
		imageName += Json["imageName"].get<std::string>();
		entt::HashedString texturePath{ imageName.c_str() };

		animationCache.load<AnimationLoader>(filename, Json, GetTexture(texturePath));
		return animationCache.handle(filename).get();
	}
	const MapResource& GetMapResource(entt::HashedString filename)
	{
		if (mapCache.contains(filename))
		{
			return mapCache.handle(filename).get();
		}
		const nlohmann::json Json = GetJson(filename);

		std::string imageName{ "Data\\Images\\Tile\\" };
		for (auto& tileset : Json["tilesets"])
		{
			imageName += tileset["image"].get<std::string>();
			break;
		}
		entt::HashedString texturePath{ imageName.c_str() };

		mapCache.load<MapLoader>(filename, Json, GetTexture(texturePath));
		return mapCache.handle(filename).get();
	}
	const sf::Font& GetFont(entt::HashedString filename)
	{
		if (fontCache.contains(filename))
		{
			return fontCache.handle(filename).get();
		}

		fontCache.load<FontLoader>(filename, filename);
		return fontCache.handle(filename).get();
	}
private:
	const nlohmann::json GetJson(entt::HashedString filename) const
	{
		std::ifstream input(static_cast<const char *>(filename));
		nlohmann::json Json;
		input >> Json;
		input.close();
		return Json;
	}
private:
	entt::ResourceCache<sf::Texture> textureCache;
	entt::ResourceCache<sf::Font> fontCache;
	entt::ResourceCache<AnimationResource> animationCache;
	entt::ResourceCache<MapResource> mapCache;
};
