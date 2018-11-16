#pragma once
#include "SFML/Graphics.hpp"
#include <assert.h>
class Graphics
{
public:
	Graphics(sf::RenderWindow& window)
		:
		window(window)
	{
		/*define it as a rectangle, located at (10, 10) and with size 100x100
		quad[0].position = sf::Vector2f(10.f, 10.f);
		quad[1].position = sf::Vector2f(110.f, 10.f);
		quad[2].position = sf::Vector2f(110.f, 110.f);
		quad[3].position = sf::Vector2f(10.f, 110.f);

		quad[4].position = sf::Vector2f(115.f, 115.f);
		quad[5].position = sf::Vector2f(215.f, 115.f);
		quad[6].position = sf::Vector2f(215.f, 215.f);
		quad[7].position = sf::Vector2f(115.f, 215.f);

		 define its texture area to be a 25x50 rectangle starting at (0, 0)
		quad[0].texCoords = sf::Vector2f(0.f, 0.f);
		quad[1].texCoords = sf::Vector2f(25.f, 0.f);
		quad[2].texCoords = sf::Vector2f(25.f, 25.f);
		quad[3].texCoords = sf::Vector2f(0.f, 25.f);

		quad[4].texCoords = sf::Vector2f(0.f, 0.f);
		quad[5].texCoords = sf::Vector2f(25.f, 0.f);
		quad[6].texCoords = sf::Vector2f(25.f, 25.f);
		quad[7].texCoords = sf::Vector2f(0.f, 25.f);


		 define the position of the triangle's points
		triangle[0].position = sf::Vector2f(10.f, 10.f);
		triangle[1].position = sf::Vector2f(100.f, 10.f);
		triangle[2].position = sf::Vector2f(100.f, 100.f);

		 define the color of the triangle's points
		triangle[0].color = sf::Color::Red;
		triangle[1].color = sf::Color::Blue;
		triangle[2].color = sf::Color::Green;

		if (!texture.loadFromFile("Data\\Images\\enemy_01.png"))
		{
			assert(false);
		}
		texture.setSmooth(true);
		******************************************************************************

		sprite.setTexture(texture);

		 position
		sprite.setPosition(sf::Vector2f(10.f, 50.f)); // absolute position
		sprite.move(sf::Vector2f(5.f, 10.f)); // offset relative to the current position

		 rotation
		sprite.setRotation(90.f); // absolute angle
		sprite.rotate(15.f); // offset relative to the current angle

		 scale
		sprite.setScale(sf::Vector2f(0.5f, 2.f)); // absolute scale factor
		sprite.scale(sf::Vector2f(1.5f, 3.f)); // factor relative to the current scale
		********************************************************************************/
	}

	void Draw(sf::VertexArray& vertexArray, const sf::Texture& texture)
	{
		//window.draw(sprite);
		//window.draw(triangle);
		window.draw(vertexArray, &texture);
	}
	void BeginFrame()
	{
		window.clear(sf::Color::Black);
	}
	void EndFrame()
	{
		window.display();
	}
private:
	sf::RenderWindow& window;
};