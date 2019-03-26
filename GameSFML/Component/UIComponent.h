#pragma once
#include "SFML/Graphics.hpp"
#include "entt/entt.hpp"

struct ScreenBaseUI
{
	uint32_t ownerEntity;
	//this is the location offset from central of the screen
	sf::Vector2f offsetLocaion;
};
struct OwnedUIComponent
{
	//hold all Owned UI entity
	std::vector<uint32_t> entities;
};
struct WorldBaseUI
{
	uint32_t ownerEntity;
};
struct UpdateScreenBaseUIComponent
{
	entt::Delegate<void(uint32_t, entt::DefaultRegistry&)> myDelegate;
};
struct UpdateWorldBaseUIComponent
{
	entt::Delegate<void(uint32_t, entt::DefaultRegistry&)> myDelegate;
};
struct ProgressiveBarComponent
{
	sf::Vector2f size;
	float percent = 1.0f;
	sf::Color colorBase{ sf::Color::Green };
	sf::Color colorBG{ sf::Color::Red };
};