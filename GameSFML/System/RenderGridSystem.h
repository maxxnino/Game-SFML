#pragma once
#include "../Locator.h"
#include "../System/ISystemECS.h"
#include "../System/IDrawSystem.h"
class GridUpdateSystem final : public ISystemECS
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		if (Locator::Graphic::empty() || Locator::Grid::empty()) return;

		auto viewport = Locator::Graphic::ref().GetViewportScreen();
		Locator::Grid::ref().Culling(viewport.first, viewport.second);
	}
};
class RenderGridSystem final : public IDrawSystem
{
public:
	void Draw(Graphics& gfx, entt::DefaultRegistry& ECS) const final
	{
		if (Locator::Grid::empty()) return;

		gfx.Draw(Locator::Grid::ref());
	}
};