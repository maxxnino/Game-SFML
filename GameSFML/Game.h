#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "TestVertex.h"
#include "World.h"
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
	sf::Clock clock;
	World wld;
};