#pragma once
#include "SFML/Graphics/Texture.hpp"
#include <entt/resource/cache.hpp>
#include "entt/core/hashed_string.hpp"
#include "Component/AnimationComponent.h"
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
struct FrameDef
{
	int holdFrame;
	int maxFrame;
	int startX;
	int startY;
	int width;
	int heigh;
};
struct FrameLoader final : entt::ResourceLoader<FrameLoader, FramesInfo> {
	std::shared_ptr<FramesInfo> load(const FrameDef& frameDef) const
	{
		auto resource = std::make_shared<FramesInfo>((float)frameDef.holdFrame / 60.0f);

		for (int i = 0; i < frameDef.maxFrame; i++)
		{
			resource->frames.emplace_back(sf::IntRect(frameDef.startX + i * frameDef.width, frameDef.startY, frameDef.width, frameDef.heigh));
		}
		return resource;
	}
};


class Codex
{
public:
	Codex()
	{
		Init();
	}
	const sf::Texture& GetTexture(entt::HashedString filename)
	{
		if (textureCache.contains(filename))
		{
			return textureCache.handle(filename).get();
		}

		textureCache.load<TextureLoader>(filename, filename);
		return textureCache.handle(filename).get();
	}
	const FramesInfo& GetFramesRect(entt::HashedString filename)
	{
		return frameCache.handle(filename).get();
	}
private:
	void Init()
	{
		std::ifstream input("Data\\Json\\animation.json");
		nlohmann::json animationJson;
		input >> animationJson;
		input.close();
		//load texture
		{
			std::string filePath = animationJson["filePath"].get<std::string>();
			entt::HashedString filename{ filePath.c_str() };
			textureCache.load<TextureLoader>(filename, filename);
		}

		//load animation
		{
			FrameDef def{
			animationJson["holdFrame"].get<int>(),
			animationJson["maxFrame"].get<int>(),
			animationJson["startX"].get<int>(),
			0,
			animationJson["width"].get<int>(),
			animationJson["heigh"].get<int>(),
			};

			std::vector<std::string> animState;
			animState.emplace_back("PlayerUp");
			animState.emplace_back("PlayerDown");
			animState.emplace_back("PlayerLeft");
			animState.emplace_back("PlayerRight");
			for (size_t i = 0; i < 4; i++)
			{
				def.startY = animationJson[animState[i]]["startY"].get<int>();
				entt::HashedString identifier{ animState[i].c_str() };
				frameCache.load<FrameLoader>(identifier, def);
			}
		}
	}
private:
	entt::ResourceCache<sf::Texture> textureCache;
	entt::ResourceCache<FramesInfo> frameCache;
};
