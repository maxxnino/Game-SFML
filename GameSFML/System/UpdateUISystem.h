#pragma once
#include "../Locator.h"
#include "../System/ISystemECS.h"
#include "../Component/UIComponent.h"

class UpdateUISystem : public ISystemECS
{
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		auto view = ECS.view<UpdateUIComponent>();
		std::for_each(std::execution::par, view.begin(), view.end(), [&ECS](auto entity) {
			auto& updateUI = ECS.get<UpdateUIComponent>(entity);
			if (updateUI.myDelegate.empty()) return;

			updateUI.myDelegate(entity, ECS);
		});
	}
};