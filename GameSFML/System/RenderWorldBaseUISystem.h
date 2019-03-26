#pragma once
#include "../Locator.h"
#include "../System/IDrawSystem.h"
#include "../Component/UIComponent.h"
#include "../Component/PhysicComponent.h"
class RenderWorldBaseUISystem final : public IDrawSystem
{
public:
	void Draw(Graphics& gfx, entt::DefaultRegistry& ECS) const final
	{
		ECS.view<UIViewable, WorldBaseUI, ProgressiveBarComponent>().each([&ECS, &gfx](auto entity, auto&, WorldBaseUI& UI, ProgressiveBarComponent& bar) {
			if (!ECS.has<sf::Sprite>(UI.ownerEntity)) return;
			auto& sprite = ECS.get<sf::Sprite>(UI.ownerEntity);
			auto location = sprite.getPosition() - sf::Vector2f(0.5f * bar.size.x, (float)sprite.getTextureRect().height);
			sf::RectangleShape rect;
			rect.setPosition(location);

			//draw background
			rect.setSize(bar.size);
			rect.setFillColor(bar.colorBG);
			gfx.Draw(rect);

			//draw base
			rect.setSize({ bar.size.x * bar.percent, bar.size.y });
			rect.setFillColor(bar.colorBase);
			gfx.Draw(rect);
		});

		ECS.view<UIViewable, WorldBaseUI, sf::Text>().each([&gfx, &ECS](auto entity, auto&, WorldBaseUI& UI, sf::Text& text) {
			if (!ECS.has<sf::Sprite>(UI.ownerEntity)) return;
			auto& sprite = ECS.get<sf::Sprite>(UI.ownerEntity);
			auto location = sprite.getPosition() - sf::Vector2f(0.5f * text.getLocalBounds().width, (float)sprite.getTextureRect().height);
			text.setPosition(location);
			gfx.Draw(text);
		});
	}
};