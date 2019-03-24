#pragma once
#include "System/ISystemECS.h"
#include "Component/TransitionStateComponent.h"
class TransitionStateSystem : public ISystemECS
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		auto view = ECS.view<TransitionStateComponent>();
		std::for_each(std::execution::par, view.begin(), view.end(), [&ECS](auto entity) {
			ECS.get<TransitionStateComponent>(entity).transitionRule.publish(entity, ECS);
		});
	}
};