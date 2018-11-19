#pragma once
#include "SFML/Graphics.hpp"
#include "Box2D/Box2D.h"
#include <assert.h>
class Graphics
{
public:
	Graphics(sf::RenderWindow& window)
		:
		window(window),
		vertices(sf::Quads, 4)
	{}

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
	std::pair<b2Vec2, b2Vec2> GetViewport()
	{
		const auto center = window.getView().getCenter();
		halfSize = 0.5f * window.getView().getSize();
		const auto top = ScreenToWorldPos(center - halfSize);
		const auto bottom = ScreenToWorldPos(center + halfSize);
		/*auto draw01 = GetDrawPosition(top) + sf::Vector2f(10.0f,10.0f);
		auto draw02 = GetDrawPosition(bottom) - sf::Vector2f(10.0f, 10.0f);
		vertices[0] = draw01;
		vertices[1] = sf::Vector2f(draw01.x,draw02.y);
		vertices[2] = draw02;
		vertices[3] = sf::Vector2f(draw02.x, draw01.y);
		window.draw(vertices);*/
		return { top, bottom };
	}
	b2Vec2 ScreenToWorldPos(const sf::Vector2f& screenPos) const
	{
		return b2Vec2((screenPos.x - halfSize.x) / scalePixel, (-screenPos.y + halfSize.y) / scalePixel);
	}
	sf::Vector2f GetDrawPosition(const b2Vec2& position) const
	{
		return { halfSize.x + position.x * scalePixel, halfSize.y - position.y * scalePixel};
	}
	static constexpr float scalePixel = 20.0f;
private:
	sf::RenderWindow& window;
	sf::Vector2f halfSize;
	sf::VertexArray vertices;
};