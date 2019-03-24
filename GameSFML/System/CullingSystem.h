#pragma once
#include "Locator.h"
#include "System/ISystemECS.h"
#include "Component/PhysicComponent.h"
#include "Component/AnimationComponent.h"
#include "Component/GameplayTags.h"
#include "entt/entt.hpp"
#include "HashStringDataBase.h"
#include <algorithm>
#include <execution>
#include <iostream>
class CullingSystem : public ISystemECS
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
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
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
	}
private:
	CullingQuerySelector cullingQueryCallback;
};