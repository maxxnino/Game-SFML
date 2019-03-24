#pragma once

#include "entt/resource/cache.hpp"
#include "GameResource.h"
#include "json.hpp"
#include <fstream>
#include <sstream>
struct TextureLoader final : entt::ResourceLoader<TextureLoader, sf::Texture> {
	std::shared_ptr<sf::Texture> load(entt::HashedString filename) const
	{
		auto texture = std::make_shared<sf::Texture>();

		if (!texture->loadFromFile(static_cast<const char *>(filename)))
		{
			assert(false);
		}
		
		return texture;
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
		for (auto& set : Json["animationSets"])
		{
			resource->animationSets.emplace_back(set["index"].get<std::pair<unsigned int, unsigned int>>());
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
				resource->layers.emplace_back(layer["data"].get<std::vector<unsigned int>>());
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
	entt::ResourceCache<AnimationResource> animationCache;
	entt::ResourceCache<MapResource> mapCache;
};
