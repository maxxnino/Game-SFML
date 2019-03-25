#pragma once
#include "../Locator.h"
#include "../System/IDrawSystem.h"
#include "../Component/UIComponent.h"
class RenderScreenBaseUISystem : public IDrawSystem
{
public:
	void Draw(Graphics& gfx) const final
	{
		if (Locator::ECS::empty()) return;

		Locator::ECS::ref().view<ScreenBaseUI, sf::Text>().each([&gfx](auto entity, ScreenBaseUI& UI, sf::Text& text) {
			gfx.Draw(text);
		});
	}
};