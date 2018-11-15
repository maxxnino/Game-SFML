#pragma once
#include "SFML/Graphics.hpp"
#include <assert.h>
class Graphics
{
	friend class MainWindow;
public:
	Graphics(sf::RenderWindow& window)
		:
		window(window)
	{}
	
	void Draw()
	{
		
	}	
private:
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