#pragma once
#include "../System/ISystemECS.h"
#include "../Component/TransitionStateComponent.h"
#include "../Component/PlayerStateComponent.h"
class TransitionStateSystem final : public ISystemECS
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		auto view = ECS.view<TransitionStateComponent>();
		std::for_each(std::execution::par, view.begin(), view.end(), [&ECS](auto entity) {
			auto& respond = ECS.get<TransitionStateComponent>(entity);
			if (respond.myDelegate.empty()) return;

			respond.myDelegate(entity, ECS);
		});
	}
};