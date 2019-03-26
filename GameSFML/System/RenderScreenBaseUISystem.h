#pragma once
#include "../Locator.h"
#include "../System/IDrawSystem.h"
#include "../Component/UIComponent.h"
class RenderScreenBaseUISystem final : public IDrawSystem
{
public:
	void Draw(Graphics& gfx, entt::DefaultRegistry& ECS) const final
	{
		ECS.view<ScreenBaseUI, ProgressiveBarComponent>().each([&gfx](auto entity, ScreenBaseUI& UI, ProgressiveBarComponent& bar) {
			const auto location = gfx.GetViewportLocation() + UI.offsetLocaion;

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

		ECS.view<ScreenBaseUI, sf::Text>().each([&gfx](auto entity, ScreenBaseUI& UI, sf::Text& text) {
			gfx.Draw(text);
		});
	}
};