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
	struct CullingQuerySelector final : public b2QueryCallback
	{
		bool ReportFixture(b2Fixture* fixture) final
		{
			foundBodies.emplace_back(fixture->GetBody()->GetUserEntity());
			return true;//keep going to find all fixtures in the query area
		}
		void Sort()
		{
			std::sort(std::execution::par_unseq, foundBodies.begin(), foundBodies.end());
		}
		std::vector<uint32_t> foundBodies;
	};
public:
	void Draw()
	{
		Culling();
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
private:
	void Culling()
	{
		auto& ECS = Locator::ECS::ref();
		ECS.reset<Viewable>();
		cullingQueryCallback.foundBodies.clear();
		auto viewport = Locator::Graphic::ref().GetViewport();
		b2AABB aabb;
		if (viewport.first.x > viewport.second.x)
		{
			std::swap(viewport.first.x, viewport.second.x);
		}
		if (viewport.first.y > viewport.second.y)
		{
			std::swap(viewport.first.y, viewport.second.y);
		}
		aabb.lowerBound = viewport.first;
		aabb.upperBound = viewport.second;
		Locator::Physic::ref().QueryAABB(&cullingQueryCallback, aabb);

		for (auto e : cullingQueryCallback.foundBodies)
		{
			ECS.assign<Viewable>(e);
		}
		std::cout << "Object on screen: "<< cullingQueryCallback.foundBodies.size() << std::endl;
	}
private:
	CullingQuerySelector cullingQueryCallback;
};