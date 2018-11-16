#include "Game.h"
#include "MainWindow.h"
Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd.window)
{
	wld.AddPlayer();
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
