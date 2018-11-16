#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "TestVertex.h"
#include "World.h"
#include <random>
class MainWindow;
class Game
{
public:
	Game(MainWindow& wnd);
	void Go();
private:
	void UpdateModel();
	void ComposeFrame();
private:
	MainWindow& wnd;
	Graphics gfx;
	/**************** User Variable *******************/
	sf::Clock clock;
	std::mt19937 rng = std::mt19937(std::random_device{}());
	World wld;
	/**************** User Variable *******************/
};