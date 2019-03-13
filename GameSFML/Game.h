#pragma once
#include "World.h"

class Game
{
public:
	void Go()
	{
		Locator::Graphic::ref().BeginFrame();
		UpdateModel();
		ComposeFrame();
		Locator::Graphic::ref().EndFrame();
	}
private:
	void UpdateModel()
	{
		const float dt = float(clock.restart().asMilliseconds()) / 1000.0f;
		wld.Update(dt);
	}
	void ComposeFrame()
	{
		wld.Draw();
	}
private:
	/**************** User Variable *******************/
	sf::Clock clock;
	World wld;
	/**************** User Variable *******************/
};