#pragma once
#include "MainWindow.h"
#include "Graphics.h"
#include "World.h"
#include "CameraSystem.h"
#include <random>

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
	CameraSystem camSystem;
	/**************** User Variable *******************/
};