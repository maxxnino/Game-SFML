#pragma once
#include <sstream>
#include "Locator.h"
#include "Component/PlayerStateComponent.h"
#include "Component/PlayerControllerComponent.h"
#include "Component/AnimationComponent.h"
#include "Component/UIComponent.h"
#include "Component/HealthComponent.h"
struct UpdateState
{
	static void Player(uint32_t entity, entt::DefaultRegistry& ECS)
	{
		if (!ECS.has<PlayerStateComponent>(entity) || !ECS.has<PlayerControllerComponent>(entity)) return;

		auto& state = ECS.get<PlayerStateComponent>(entity);
		auto& controller = ECS.get<PlayerControllerComponent>(entity);
		if (controller.direction == sf::Vector2i(0, 0))
		{
			//if (state.state == PlayerStateComponent::State::Standing) return;

			state.state = PlayerStateComponent::State::Standing;

			if (!ECS.has<AnimationComponent>(entity)) return;

			auto& animation = ECS.get<AnimationComponent>(entity);
			animation.curFrameTime = 0.0f;
			animation.iCurFrame = animation.rangeIndex.first;
			animation.rangeIndex.second = animation.rangeIndex.first;
		}
		else
		{
			if (controller.direction.x < 0)
			{
				if (state.direction == PlayerStateComponent::Direction::Left) return;

				state.direction = PlayerStateComponent::Direction::Left;

				if (!ECS.has<AnimationComponent>(entity)) return;
				auto& animation = ECS.get<AnimationComponent>(entity);

				animation.curFrameTime = 0.0f;
				animation.rangeIndex = animation.resource->animationSets[static_cast<unsigned int>(state.direction)];
				animation.iCurFrame = animation.rangeIndex.first;
				return;
			}

			if (controller.direction.x > 0)
			{
				if (state.direction == PlayerStateComponent::Direction::Right) return;

				state.direction = PlayerStateComponent::Direction::Right;
				if (!ECS.has<AnimationComponent>(entity)) return;
				auto& animation = ECS.get<AnimationComponent>(entity);

				animation.curFrameTime = 0.0f;
				animation.rangeIndex = animation.resource->animationSets[static_cast<unsigned int>(state.direction)];
				animation.iCurFrame = animation.rangeIndex.first;
				return;
			}

			if (controller.direction.y < 0)
			{
				if (state.direction == PlayerStateComponent::Direction::Down) return;

				state.direction = PlayerStateComponent::Direction::Down;
				if (!ECS.has<AnimationComponent>(entity)) return;
				auto& animation = ECS.get<AnimationComponent>(entity);

				animation.curFrameTime = 0.0f;
				animation.rangeIndex = animation.resource->animationSets[static_cast<unsigned int>(state.direction)];
				animation.iCurFrame = animation.rangeIndex.first;
				return;
			}

			if (controller.direction.y > 0)
			{
				if (state.direction == PlayerStateComponent::Direction::Up) return;

				state.direction = PlayerStateComponent::Direction::Up;
				if (!ECS.has<AnimationComponent>(entity)) return;
				auto& animation = ECS.get<AnimationComponent>(entity);

				animation.curFrameTime = 0.0f;
				animation.rangeIndex = animation.resource->animationSets[static_cast<unsigned int>(state.direction)];
				animation.iCurFrame = animation.rangeIndex.first;
				return;
			}
		}
	}
};
struct CollisionRespond
{
	static void Player(uint32_t entity, entt::DefaultRegistry& ECS)
	{
		auto& callbackData = ECS.get<CollisionCallbackData>(entity);

		for (auto& other : callbackData.others)
		{
			if (other.second == CollisionFillter::PLAYER) continue;

			if (other.second == CollisionFillter::ENEMY)
			{
				if (ECS.has<HealthComponent>(entity))
				{
					ECS.get<HealthComponent>(entity).curHealth -= 1.0f;
				}
			}
		}
	}
	static void Enemy(uint32_t entity, entt::DefaultRegistry& ECS)
	{
		auto& callbackData = ECS.get<CollisionCallbackData>(entity);

		for (auto& other : callbackData.others)
		{
			if (other.second == CollisionFillter::ENEMY) continue;

			if (other.second == CollisionFillter::PLAYER)
			{
				if (ECS.has<HealthComponent>(entity))
				{
					ECS.get<HealthComponent>(entity).curHealth -= 5.0f;
				}
			}
		}
	}
	static void Bullet(uint32_t entity, entt::DefaultRegistry& ECS)
	{
		auto& callbackData = ECS.get<CollisionCallbackData>(entity);

		for (auto& other : callbackData.others)
		{
			if (other.second == CollisionFillter::BULLET) continue;

			if (other.second == CollisionFillter::ENEMY)
			{
				if (ECS.has<HealthComponent>(entity))
				{
					ECS.get<HealthComponent>(entity).curHealth = 0.0f;
				}
				if (ECS.has<HealthComponent>(other.first))
				{
					ECS.get<HealthComponent>(other.first).curHealth -= 20.0f;
				}
			}
		}
	}
};
struct UpdateUI
{
	static void ScreenBaseHealthText(uint32_t entity, entt::DefaultRegistry& ECS)
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
	static void ScreenBaseHealthBar(uint32_t entity, entt::DefaultRegistry& ECS)
	{
		if (!ECS.has<ProgressiveBarComponent>(entity) || !ECS.has<ScreenBaseUI>(entity)) return;

		auto& UI = ECS.get<ScreenBaseUI>(entity);

		if (!ECS.has<HealthComponent>(UI.ownerEntity)) return;

		auto& progressBar = ECS.get<ProgressiveBarComponent>(entity);
		auto& health = ECS.get<HealthComponent>(UI.ownerEntity);
		progressBar.percent = health.curHealth / health.maxHealth;
	}
	static void DeleteOwnedUIComponent(entt::DefaultRegistry& ECS, uint32_t entity)
	{
		auto& owend = ECS.get<OwnedUIComponent>(entity);
		for (auto& e : owend.entities)
		{
			ECS.destroy(e);
		}
	}
};
struct UIFactory
{
	static uint32_t GetEntity(uint32_t entityOwner, entt::DefaultRegistry& ECS, bool bIsHaveOwner = true)
	{
		if (bIsHaveOwner)
		{
			auto uiEntity = ECS.create();
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

		return ECS.create();
	}
};
