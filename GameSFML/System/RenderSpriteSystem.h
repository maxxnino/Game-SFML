#pragma once
#include "Locator.h"
#include "System/ISystemECS.h"
#include "System/IDrawSystem.h"
#include "Component/PhysicComponent.h"
#include "Component/AnimationComponent.h"
#include "Component/GameplayTags.h"
#include "entt/entt.hpp"
#include "HashStringDataBase.h"
#include <algorithm>
#include <execution>
#include <iostream>
class RenderSpriteSystem : public ISystemECS, public IDrawSystem
{
public:
	//update before Culling, animation, physic
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		{
			auto view = ECS.view<Viewable, AnimationComponent, sf::Sprite>();

			std::for_each(std::execution::par, view.begin(), view.end(), [&ECS](auto entity) {
				auto& animCom = ECS.get<AnimationComponent>(entity);
				ECS.get<sf::Sprite>(entity).setTextureRect(animCom.frames->at(animCom.iCurFrame));
			});
		}

		{
			auto view = ECS.view<Viewable, PhysicComponent, sf::Sprite>();
			std::for_each(std::execution::par, view.begin(), view.end(), [&ECS](auto entity) {
				auto& sprite = ECS.get<sf::Sprite>(entity);

				sprite.setPosition(
					Locator::Graphic::ref().WorldToScreenPos(ECS.get<PhysicComponent>(entity).body->GetPosition())
				);
			});
		}
	}
	void Draw(Graphics& gfx) const final
	{
		auto& ECS = Locator::ECS::ref();
		ECS.view<Viewable, sf::Sprite>().each([&gfx](auto entity, auto&, sf::Sprite &sprite) {
			gfx.DrawSprite(sprite);
		});
	}
};