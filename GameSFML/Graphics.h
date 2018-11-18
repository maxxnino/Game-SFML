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
		/******************************************************************************

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
	const sf::FloatRect& GetViewport() const
	{
		return window.getView().getViewport();
	}
private:
	sf::RenderWindow& window;
};