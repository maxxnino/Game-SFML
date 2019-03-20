#pragma once
#include "Locator.h"
class RenderGridSystem
{
public:
	void Draw()
	{
		auto& gfx = Locator::Graphic::ref();
		auto& grid = Locator::Grid::ref();

		auto viewport = gfx.GetViewportScreen();
		grid.Culling(viewport.first, viewport.second);

		gfx.Draw(grid);
	}
};