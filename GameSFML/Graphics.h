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

	void DrawVertexArray(sf::VertexArray& vertexArray, const sf::Texture& texture) const
	{
		window.draw(vertexArray, &texture);
	}
	void DrawSprite(sf::Sprite& sprite) const
	{
		window.draw(sprite);
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
		/*auto draw01 = WorldToScreenPos(top) + sf::Vector2f(10.0f,10.0f);
		auto draw02 = WorldToScreenPos(bottom) - sf::Vector2f(10.0f, 10.0f);
		vertices[0] = draw01;
		vertices[1] = sf::Vector2f(draw01.x,draw02.y);
		vertices[2] = draw02;
		vertices[3] = sf::Vector2f(draw02.x, draw01.y);
		window.draw(vertices);*/
		return { top, bottom };
	}
	void MoveViewport(const b2Vec2& newPos)
	{
		auto view = window.getView();
		view.setCenter(WorldToScreenPos(newPos));
		window.setView(view);
	}
	b2Vec2 ScreenToWorldPos(const sf::Vector2f& screenPos) const
	{
		return b2Vec2((screenPos.x - halfSize.x) / scalePixel, (-screenPos.y + halfSize.y) / scalePixel);
	}
	sf::Vector2f WorldToScreenPos(const b2Vec2& worldPos) const
	{
		return { halfSize.x + worldPos.x * scalePixel, halfSize.y - worldPos.y * scalePixel};
	}
	sf::RenderWindow& GetRenderWindow()
	{
		return window;
	}
	static constexpr float scalePixel = 20.0f;
private:
	sf::RenderWindow& window;
	sf::Vector2f halfSize;
	sf::VertexArray vertices;
};