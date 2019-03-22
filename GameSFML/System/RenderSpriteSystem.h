#pragma once
#include "Locator.h"
#include "Component/PhysicComponent.h"
#include "Component/AnimationComponent.h"
#include "Component/GameplayTags.h"
#include "entt/entt.hpp"
#include "HashStringDataBase.h"
#include <algorithm>
#include <execution>
#include <iostream>
class RenderSpriteSystem
{
public:
	void Draw()
	{
		auto& ECS = Locator::ECS::ref();
		ECS.view<Viewable, AnimationComponent, sf::Sprite>().each([&ECS](auto entity, auto&, AnimationComponent& animCom,sf::Sprite &sprite) {
			sprite.setTextureRect(animCom.frames->at(animCom.iCurFrame));
		});

		ECS.view<Viewable, PhysicComponent, sf::Sprite>().each([](auto entity, auto&, PhysicComponent &physic, sf::Sprite &sprite) {
			sprite.setPosition(
				Locator::Graphic::ref().WorldToScreenPos(physic.body->GetPosition())
			);
			Locator::Graphic::ref().DrawSprite(sprite);
		});

	}
};