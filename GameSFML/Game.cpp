#include "Game.h"
#include "MainWindow.h"
Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd.window)
{
	gfx.MoveViewport(b2Vec2(0.0f, 0.0f));
	std::uniform_real_distribution<float> rangeX(-20.0f, 20.0f);
	std::uniform_real_distribution<float> rangeY(-20.0f, 20.0f);
	std::uniform_int_distribution<int> range(0, 3);

	
}

void Game::Go()
{
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = float(clock.restart().asMilliseconds()) / 1000.0f;
	wld.Update(dt, wnd.kbd, wnd.mouse, gfx);
}

void Game::ComposeFrame()
{
	wld.Draw(gfx);
}
