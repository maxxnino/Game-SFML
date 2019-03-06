#pragma once
#include "Graphics.h"
#include "PhysicComponent.h"
#include "entt/entt.hpp"
#include "HashStringDataBase.h"
#include <algorithm>
#include <execution>
struct Drawable{};
class CullingObject
{
	//query for select by point
	class CullingQuerySelector final : public b2QueryCallback
	{
	public:
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
	const std::vector<uint32_t>& GetEntityList(b2World& box2DEngine, b2Vec2 p01, b2Vec2 p02)
	{
		cullingQueryCallback.foundBodies.clear();
		b2AABB aabb;
		if (p01.x > p02.x)
		{
			std::swap(p01.x, p02.x);
		}
		if (p01.y > p02.y)
		{
			std::swap(p01.y, p02.y);
		}
		aabb.lowerBound = p01;
		aabb.upperBound = p02;
		box2DEngine.QueryAABB(&cullingQueryCallback, aabb);
		//cullingQueryCallback.Sort();
		return cullingQueryCallback.foundBodies;
	}
private:
	CullingQuerySelector cullingQueryCallback;
};
class RenderSpriteSystem
{
public:
	void Draw(entt::DefaultRegistry& ECSEngine, Graphics& gfx)
	{
		ECSEngine.view<sf::Sprite, PhysicComponent>().each([&gfx](auto entity, sf::Sprite &sprite, PhysicComponent &physic) {
			sprite.setPosition(gfx.WorldToScreenPos(physic.body->GetPosition()));
			gfx.DrawSprite(sprite);
		});
	}
	void Update(entt::DefaultRegistry& ECSEngine, Graphics& gfx, b2World& box2DEngine)
	{
		auto viewport = gfx.GetViewport();
		auto entities = cullingObject.GetEntityList(box2DEngine, viewport.first, viewport.second);
		//std::cout << entities.size() << "\n";
		for (auto e : entities)
		{
			ECSEngine.assign<Drawable>(e);
		}
	}
private:
	CullingObject cullingObject;
};