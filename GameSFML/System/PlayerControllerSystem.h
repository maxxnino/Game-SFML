#pragma once
#include "../Locator.h"
#include "../System/ISystemECS.h"
#include "../Component/PlayerControllerComponent.h"
#include "../MaxxConsole.h"
class PlayerControllerSystem final : public ISystemECS
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		//keyboard
		auto& kbd = Locator::Keyboard::ref();
		sf::Vector2i dir{ 0,0 };
		if (kbd.KeyIsPressed(sf::Keyboard::A))
		{
			dir.x = -1;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::D))
		{
			dir.x = 1;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::S))
		{
			dir.y = -1;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::W))
		{
			dir.y = 1;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::Space))
		{
			if (MaxxConsole::r_showDebugPhysic == 0)
			{
				MaxxConsole::r_showDebugPhysic = 1;
			}
			else
			{
				MaxxConsole::r_showDebugPhysic = 0;
			}
		}
		//mouse
		auto& mouse = Locator::Mouse::ref();
		bool bIsShooting = false;
		while (!mouse.IsEmpty())
		{
			auto e = mouse.Read();
			if (e.GetType() == Mouse::Event::Type::LPress)
			{
				bIsShooting = true;
				continue;
			}
			/*if (e.GetType() == Mouse::Event::Type::RPress)
			{

				continue;
			}*/
		}
		ECS.view<PlayerControllerComponent>().each([&dir, bIsShooting, &mouse](auto entity, PlayerControllerComponent& controller) {
			controller.direction = dir;
			controller.bIsShooting = bIsShooting;
			controller.mousePos = mouse.GetPos();
		});
	}
};