#pragma once
#include "SFML/Graphics/Texture.hpp"
#include <entt/resource/cache.hpp>
#include "entt/core/hashed_string.hpp"

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


class Codex
{
public:
	static const sf::Texture& GetTexture(entt::HashedString filename)
	{
		return Get().LoadOrRetrieveTexture(filename);
	}
private:
	static Codex& Get()
	{
		static Codex codex;
		return codex;
	}
	const sf::Texture& LoadOrRetrieveTexture(entt::HashedString filename)
	{
		textureCache.load<TextureLoader>(filename, filename);
		return textureCache.handle(filename).get();
	}

private:
	entt::ResourceCache<sf::Texture> textureCache;
};
