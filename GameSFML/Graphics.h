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
	{
		MoveViewport(b2Vec2(0.0f,0.0f));
	}

	void DrawVertexArray(sf::VertexArray& vertexArray, const sf::Texture& texture) const
	{
		window.draw(vertexArray, &texture);
	}
	void Draw(const sf::Drawable& drawable) const
	{
		window.draw(drawable);
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
	std::pair<sf::Vector2f, sf::Vector2f> GetViewportScreen()
	{
		const auto center = window.getView().getCenter();
		halfSize = 0.5f * window.getView().getSize();
		return { center - halfSize , center + halfSize };
	}
	void MoveViewport(const b2Vec2& newWorldPos)
	{
		auto view = window.getView();
		view.setCenter(WorldToScreenPos(newWorldPos));
		window.setView(view);
	}
	void MoveViewport(const sf::Vector2i screenPos)
	{
		auto view = window.getView();

		view.setCenter(view.getCenter() + (sf::Vector2f)screenPos);
		window.setView(view);
	}
	b2Vec2 ScreenToWorldPos(const sf::Vector2f& screenPos) const
	{
		return b2Vec2((screenPos.x) / scalePixel, (-screenPos.y) / scalePixel);
	}
	sf::Vector2f WorldToScreenPos(const b2Vec2& worldPos) const
	{
		return { worldPos.x * scalePixel, - worldPos.y * scalePixel};
	}
	b2Vec2 GetWorldSize(const sf::Vector2f& screenSize) const
	{
		return b2Vec2(screenSize.x / scalePixel, screenSize.y / scalePixel);
	}
	sf::Vector2f GetScreenSize(const b2Vec2& worldSize) const
	{
		return { worldSize.x * scalePixel, worldSize.y * scalePixel };
	}
	static constexpr float scalePixel = 20.0f;
private:
	sf::RenderWindow& window;
	sf::Vector2f halfSize;
	sf::VertexArray vertices;
};