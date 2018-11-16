#include "Game.h"
#include "MainWindow.h"
Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd.window)
{
	std::uniform_real_distribution<float> rangeX(100.0f, 1180.0f);
	std::uniform_real_distribution<float> rangeY(100.0f, 620.0f);
	for (size_t i = 0; i < 100000; i++)
	{
		wld.AddPlayer({ rangeX(rng),rangeY(rng) });
	}
	
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
	wld.Update(dt, wnd.kbd, wnd.mouse);
}

void Game::ComposeFrame()
{
	wld.Draw(gfx);
}
