#pragma once
#include "SFML/Graphics.hpp"
#include "entt/entt.hpp"
#include <string>

class Codex
{
	struct TextureLoader final : entt::ResourceLoader<TextureLoader, sf::Texture>
	{
		std::shared_ptr<sf::Texture> load(const std::string& filename) const
		{
			auto resource = std::make_shared<sf::Texture>();
			if (!resource->loadFromFile(filename))
			{
				assert(false);
			}
			resource->setSmooth(true);
			return resource;
		}
	};
	/*struct IntRectLoader final : entt::ResourceLoader<IntRectLoader, std::vector<sf::IntRect>>
	{
		std::shared_ptr<std::vector<sf::IntRect>> load(int x, int y, int width, int height, int nRect ) const
		{
			auto resource = std::make_shared<std::vector<sf::IntRect>>();
			for (int i = 0; i < nRect; i++)
			{
				resource->emplace_back(sf::IntRect(x + i * width, y, width, height));
			}
			return resource;
		}
	};*/
public:
	const sf::Texture& GetTexture(const entt::HashedString& identifier)
	{
		return textureCache.handle(identifier).get();
	}
	
	void AddTexture(const entt::HashedString& indentifier, const std::string& filename)
	{
		textureCache.load<TextureLoader>(indentifier, filename);
	}

	/*const std::vector<sf::IntRect>& GetIntRect(const entt::HashedString& identifier)
	{
		return intRectCache.handle(identifier).get();
	}
	void AddIntRect(const entt::HashedString& indentifier, int x, int y, int width, int height, int nRect)
	{
		intRectCache.load<IntRectLoader>(indentifier, x, y, width, height, nRect);
	}*/
private:
	entt::ResourceCache<sf::Texture> textureCache{};
	//entt::ResourceCache<std::vector<sf::IntRect>> intRectCache{};
};