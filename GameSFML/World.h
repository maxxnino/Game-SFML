#pragma once
#include "Mouse.h"
#include <entt/entity/registry.hpp>
#include "Codex.h"
#include "HashStringDataBase.h"
class World
{
public:
	World()
	{
		box2DEngine = std::make_unique<b2World>(b2Vec2(0.0f, 0.0f));
		sprite.setTexture(Codex::GetTexture(Database::TexEnemy01));
	}
	void Update(float dt, Keyboard& kbd, Mouse& mouse, Graphics& gfx)
	{
		
	}
	void Draw(Graphics& gfx)
	{
		gfx.DrawSprite(sprite);
	}
	
private:
	std::unique_ptr<b2World> box2DEngine;
	entt::DefaultRegistry ECSWorld;
	sf::Sprite sprite;
};