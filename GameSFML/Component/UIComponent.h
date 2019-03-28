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

	static void Destruction(entt::DefaultRegistry& ECS, uint32_t entity)
	{
		auto& owend = ECS.get<OwnedUIComponent>(entity);
		for (auto& e : owend.entities)
		{
			ECS.destroy(e);
		}
	}
};
struct WorldBaseUI
{
	uint32_t ownerEntity;
};

struct ProgressiveBarComponent
{
	sf::Vector2f size;
	float percent = 1.0f;
	sf::Color colorBase{ sf::Color::Green };
	sf::Color colorBG{ sf::Color::Red };
};
struct UpdateScreenBaseUIComponent
{
	entt::Delegate<void(uint32_t, entt::DefaultRegistry&)> myDelegate;

	static void HealthText(uint32_t entity, entt::DefaultRegistry& ECS)
	{
		if (!ECS.has<sf::Text>(entity) || !ECS.has<ScreenBaseUI>(entity)) return;

		auto& UI = ECS.get<ScreenBaseUI>(entity);

		if (!ECS.has<HealthComponent>(UI.ownerEntity)) return;

		auto& text = ECS.get<sf::Text>(entity);
		auto& health = ECS.get<HealthComponent>(UI.ownerEntity);
		std::stringstream ss;
		ss << "Health: " << (int)health.curHealth << " / " << (int)health.maxHealth;
		text.setString(ss.str());
		if (Locator::Graphic::empty()) return;

		text.setPosition(Locator::Graphic::ref().GetViewportLocation() + UI.offsetLocaion);
	}
	static void HealthBar(uint32_t entity, entt::DefaultRegistry& ECS)
	{
		if (!ECS.has<ProgressiveBarComponent>(entity) || !ECS.has<ScreenBaseUI>(entity)) return;

		auto& UI = ECS.get<ScreenBaseUI>(entity);

		if (!ECS.has<HealthComponent>(UI.ownerEntity)) return;

		auto& progressBar = ECS.get<ProgressiveBarComponent>(entity);
		auto& health = ECS.get<HealthComponent>(UI.ownerEntity);
		progressBar.percent = health.curHealth / health.maxHealth;
	}
};
struct UpdateWorldBaseUIComponent
{
	entt::Delegate<void(uint32_t, entt::DefaultRegistry&)> myDelegate;

	static void HealthText(uint32_t entity, entt::DefaultRegistry& ECS)
	{
		if (!ECS.has<sf::Text>(entity) || !ECS.has<WorldBaseUI>(entity)) return;

		auto& UI = ECS.get<WorldBaseUI>(entity);

		if (!ECS.has<HealthComponent>(UI.ownerEntity)) return;

		auto& text = ECS.get<sf::Text>(entity);
		auto& health = ECS.get<HealthComponent>(UI.ownerEntity);
		std::stringstream ss;
		ss << (int)health.curHealth << " / " << (int)health.maxHealth;
		text.setString(ss.str());
	}
	static void HealthBar(uint32_t entity, entt::DefaultRegistry& ECS)
	{
		if (!ECS.has<ProgressiveBarComponent>(entity) || !ECS.has<WorldBaseUI>(entity)) return;

		auto& UI = ECS.get<WorldBaseUI>(entity);

		if (!ECS.has<HealthComponent>(UI.ownerEntity)) return;

		auto& progressBar = ECS.get<ProgressiveBarComponent>(entity);
		auto& health = ECS.get<HealthComponent>(UI.ownerEntity);
		progressBar.percent = health.curHealth / health.maxHealth;
	}
};

struct UIFactory
{
	static uint32_t GetEntity(uint32_t entityOwner, entt::DefaultRegistry& ECS, bool bIsHaveOwner = true)
	{
		auto uiEntity = ECS.create();
		if (bIsHaveOwner)
		{

			if (ECS.has<OwnedUIComponent>(entityOwner))
			{
				auto& ownedUICom = ECS.get<OwnedUIComponent>(entityOwner);
				ownedUICom.entities.emplace_back(uiEntity);
				return uiEntity;
			}
			else
			{
				auto& ownedUICom = ECS.assign<OwnedUIComponent>(entityOwner);
				ownedUICom.entities.emplace_back(uiEntity);
				return uiEntity;
			}
		}

		return uiEntity;
	}
};