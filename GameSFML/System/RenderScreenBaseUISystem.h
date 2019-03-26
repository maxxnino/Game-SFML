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

		Locator::ECS::ref().view<ProgressiveBarComponent, ScreenBaseUI>().each([&gfx](auto entity, ProgressiveBarComponent& bar, ScreenBaseUI& UI) {
			const auto location = Locator::Graphic::ref().GetViewportLocation() + UI.offsetLocaion;

			sf::RectangleShape rectBG;
			sf::RectangleShape rectBase;

			rectBG.setPosition(location);
			rectBase.setPosition(location);
			rectBG.setSize(bar.size);
			rectBase.setSize({ bar.size.x * bar.percent, bar.size.y });
			rectBG.setFillColor(bar.colorBG);
			rectBase.setFillColor(bar.colorBase);
			gfx.Draw(rectBG);
			gfx.Draw(rectBase);
		});

		Locator::ECS::ref().view<ScreenBaseUI, sf::Text>().each([&gfx](auto entity, ScreenBaseUI& UI, sf::Text& text) {
			gfx.Draw(text);
		});

		
	}
};