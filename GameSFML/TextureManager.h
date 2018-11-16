#pragma once
#include "SFML/Graphics.hpp"
#include "entt/entt.hpp"
#include "MeshComponent.h"
#include <string>

class TextureManager
{
	struct TextureLoader final : entt::ResourceLoader<TextureLoader, sf::Texture>
	{
		std::shared_ptr<sf::Texture> load(const std::string& filename) const
		{
			auto texture = std::make_shared<sf::Texture>();
			if (!texture->loadFromFile(filename))
			{
				assert(false);
			}
			texture->setSmooth(true);
			return texture;
		}
	};
	struct MeshLoader final : entt::ResourceLoader<MeshLoader, MeshComponent>
	{
		std::shared_ptr<MeshComponent> load(float size) const
		{
			const float halfSize = size / 2.0f;
			auto mesh = std::make_shared<MeshComponent>();
			mesh->point01 = sf::Vector2f(-halfSize, -halfSize);
			mesh->point02 = sf::Vector2f(halfSize, -halfSize);
			mesh->point03 = sf::Vector2f(halfSize, halfSize);
			mesh->point04 = sf::Vector2f(-halfSize, halfSize);
			return mesh;
		}
	};
public:
	const sf::Texture& GetTexture(const entt::HashedString& identifier)
	{
		return textureCache.handle(identifier).get();
	}
	const MeshComponent& GetMesh(const entt::HashedString& identifier)
	{
		return meshCache.handle(identifier).get();
	}
	void AddTexture(const entt::HashedString& indentifier, const std::string& filename)
	{
		textureCache.load<TextureLoader>(indentifier, filename);
	}
	void AddMesh(const entt::HashedString& indentifier, float size)
	{
		meshCache.load<MeshLoader>(indentifier, size);
	}
private:
	entt::ResourceCache<sf::Texture> textureCache{};
	entt::ResourceCache<MeshComponent> meshCache{};
};