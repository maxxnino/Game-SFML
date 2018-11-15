#pragma once
#include "Keyboard.h"
#include "Mouse.h"
class Game
{
public:
	Game(Keyboard& kbd, Mouse& mouse)
		:
		kbd(kbd),
		mouse(mouse)
	{}
	void Go()
	{
		Update();
		Draw();
	}
private:
	void Update()
	{

	}
	void Draw()
	{

	}
private:
	Keyboard& kbd;
	Mouse& mouse;
};