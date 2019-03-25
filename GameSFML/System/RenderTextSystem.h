#pragma once
#include "Locator.h"
#include "ISystemECS.h"
#include "IDrawSystem.h"
#include "Component/TextComponent.h"
#include "Component/GameplayTags.h"
class RenderTextSystem : public ISystemECS, public IDrawSystem
{
private:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		ECS.view<UpdateTextLocation, TextLocation>().each([](auto entity, UpdateTextLocation& updateInfo, TextLocation& location) {
			location.location = updateInfo.newLocation;
		});
		ECS.view<UpdateTextString, sf::Text>().each([](auto entity, UpdateTextString& updateInfo, sf::Text& text) {
			text.setString(updateInfo.newString);
		});
		ECS.view<UpdateTextFont, sf::Text>().each([](auto entity, UpdateTextFont& updateInfo, sf::Text& text) {
			if (updateInfo.newFont == nullptr) return;

			text.setFont(*updateInfo.newFont);
		});
		if (Locator::Graphic::empty()) return;
		
		auto screenPosition = Locator::Graphic::ref().GetViewportLocation();
		ECS.view<ScreenBaseUI, TextLocation, sf::Text>().each([&screenPosition](auto entity, auto&, TextLocation& loc, sf::Text& text) {
			text.setPosition(loc.location + screenPosition);
		});
	}
	void Draw(Graphics& gfx) const final
	{
		if (Locator::ECS::empty()) return;

		Locator::ECS::ref().view<sf::Text>().each([&gfx](auto entity, sf::Text& text) {
			gfx.Draw(text);
		});
	}
};