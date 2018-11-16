#pragma once
#include "SFML/Graphics.hpp"
#include "Graphics.h"
#include "Keyboard.h"
class TestVertex
{
public:
	TestVertex()
		:
		baseTexArray(sf::Quads, 4),
		drawTexArray(sf::Quads, 4)
	{
		baseTexArray[0].position = sf::Vector2f(10.f, 10.f);
		baseTexArray[1].position = sf::Vector2f(110.f, 10.f);
		baseTexArray[2].position = sf::Vector2f(110.f, 110.f);
		baseTexArray[3].position = sf::Vector2f(10.f, 110.f);

		drawTexArray[0].texCoords = sf::Vector2f(0.f, 0.f);
		drawTexArray[1].texCoords = sf::Vector2f(25.f, 0.f);
		drawTexArray[2].texCoords = sf::Vector2f(25.f, 25.f);
		drawTexArray[3].texCoords = sf::Vector2f(0.f, 25.f);

		if (!texture.loadFromFile("Data\\Images\\enemy_01.png"))
		{
			assert(false);
		}
		texture.setSmooth(true);
	}
	void Update(float dt, Keyboard& kbd)
	{
		sf::Vector2f dir = { 0.0f,0.0f };
		float rotDir = 0.0f;
		
		pos += dt * speed * dir;
		sf::Transform T;
		T.translate(pos);
		rot += dt * rotSpeed * rotDir;
		sf::Transform R;
		R.rotate(rot);

		sf::Transform M = T * R;
		for (size_t i = 0; i < 4; i++)
		{
			drawTexArray[i].position = M.transformPoint(baseTexArray[i].position);
		}
	}
	void Draw(Graphics& gfx)
	{
		gfx.Draw(drawTexArray, texture);
	}
private:
	sf::VertexArray baseTexArray;
	sf::VertexArray drawTexArray;
	sf::Texture texture;
	sf::Vector2f pos = { 0.0f,0.0f };
	float rot = 0.0f;
	float speed = 100.0f;
	float rotSpeed = 90.0f;
};