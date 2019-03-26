#pragma once
#include <sstream>
#include "../Locator.h"
#include "../System/ISystemECS.h"
#include "../Component/UIComponent.h"

class UpdateScreenBaseUISystem : public ISystemECS
{
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		auto view = ECS.view<UpdateScreenBaseUIComponent>();
		std::for_each(std::execution::par, view.begin(), view.end(), [&ECS](auto entity) {
			auto& updateUI = ECS.get<UpdateScreenBaseUIComponent>(entity);
			if (updateUI.myDelegate.empty()) return;

			updateUI.myDelegate(entity, ECS);
		});
	}
};

class UpdateWorldBaseUISystem : public ISystemECS
{
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		ECS.reset<UIViewable>();

		ECS.view<Viewable, OwnedUIComponent>().each([&ECS](auto entity, auto&, OwnedUIComponent& owned) {
			for (auto e : owned.entities)
			{
				ECS.assign<UIViewable>(e);
			}
		});

		auto view = ECS.view<UIViewable, UpdateWorldBaseUIComponent>();
		std::for_each(std::execution::par, view.begin(), view.end(), [&ECS](auto entity) {
			auto& updateUI = ECS.get<UpdateWorldBaseUIComponent>(entity);
			if (updateUI.myDelegate.empty()) return;

			updateUI.myDelegate(entity, ECS);
		});
	}
};