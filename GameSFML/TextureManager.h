#pragma once
#include "SFML/Graphics.hpp"
#include "entt/entt.hpp"
#include "ModelComponent.h"
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
	struct MeshLoader final : entt::ResourceLoader<MeshLoader, ModelComponent>
	{
		std::shared_ptr<ModelComponent> load(float size, float texCoordW, float texCoordH) const
		{
			const float halfWidth = size / 2.0f;
			const float halfHeight = (size * texCoordH) / (2.0f * texCoordW);
			auto mesh = std::make_shared<ModelComponent>();
			mesh->meshP01 = sf::Vector2f(-halfWidth, -halfHeight);
			mesh->meshP02 = sf::Vector2f(halfWidth, -halfHeight);
			mesh->meshP03 = sf::Vector2f(halfWidth, halfHeight);
			mesh->meshP04 = sf::Vector2f(-halfWidth, halfHeight);
			
			mesh->texCoord01 = sf::Vector2f(0.0f, 0.0f);
			mesh->texCoord02 = sf::Vector2f(texCoordW, 0.0f);
			mesh->texCoord03 = sf::Vector2f(texCoordW, texCoordH);
			mesh->texCoord04 = sf::Vector2f(0.0f, texCoordH);
			return mesh;
		}
	};
public:
	const sf::Texture& GetTexture(const entt::HashedString& identifier)
	{
		return textureCache.handle(identifier).get();
	}
	const ModelComponent& GetMesh(const entt::HashedString& identifier)
	{
		return meshCache.handle(identifier).get();
	}
	void AddTexture(const entt::HashedString& indentifier, const std::string& filename)
	{
		textureCache.load<TextureLoader>(indentifier, filename);
	}
	void AddMesh(const entt::HashedString& indentifier, float size, float texCoordW, float texCoordH)
	{
		meshCache.load<MeshLoader>(indentifier, size, texCoordW, texCoordH);
	}
private:
	entt::ResourceCache<sf::Texture> textureCache{};
	entt::ResourceCache<ModelComponent> meshCache{};
};