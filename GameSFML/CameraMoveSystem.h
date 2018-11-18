#pragma once
#include "SFML/Graphics.hpp"
#include "Keyboard.h"
class CameraMoveSystem
{
public:
	void Update(sf::RenderWindow& wnd, Keyboard& kbd, float dt)
	{
		sf::Vector2f dir{ 0.0f,0.0f };
		//float rot = 0.0f;
		if (kbd.KeyIsPressed(sf::Keyboard::A))
		{
			dir.x = -1.0f;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::D))
		{
			dir.x = 1.0f;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::S))
		{
			dir.y = 1.0f;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::W))
		{
			dir.y = -1.0f;
		}
		/*if (kbd.KeyIsPressed(sf::Keyboard::Q))
		{
			rot = -1.0f;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::E))
		{
			rot = 1.0f;
		}*/

		auto view = wnd.getView();
		view.move(dt * speed * dir);
		//view.rotate(rot * dt * rotSpeed);
		wnd.setView(view);
	}
private:
	float speed = 100.0f;
	float rotSpeed = 30.0f;
};