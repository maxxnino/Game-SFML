#pragma once
#include "../Locator.h"
#include "../System/ISystemECS.h"
#include "../System/IDrawSystem.h"
class RenderGridSystem : public ISystemECS, public IDrawSystem
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		if (Locator::Graphic::empty() || Locator::Grid::empty()) return;

		auto viewport = Locator::Graphic::ref().GetViewportScreen();
		Locator::Grid::ref().Culling(viewport.first, viewport.second);
	}
	void Draw(Graphics& gfx) const final
	{
		if (Locator::Grid::empty()) return;

		gfx.Draw(Locator::Grid::ref());
	}
};