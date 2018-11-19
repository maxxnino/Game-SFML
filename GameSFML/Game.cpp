#include "Game.h"
#include "MainWindow.h"
Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd.window)
{
	std::uniform_real_distribution<float> rangeX(-20.0f, 20.0f);
	std::uniform_real_distribution<float> rangeY(-20.0f, 20.0f);
	std::uniform_int_distribution<int> range(0, 2);
	for (size_t i = 0; i < 50; i++)
	{
		switch (range(rng))
		{
		case 0:
			wld.AddPlayerWithTag<HashStringManager::Enemy01>({ rangeX(rng),rangeY(rng) });
			break;
		case 1:
			wld.AddPlayerWithTag<HashStringManager::Enemy02>({ rangeX(rng),rangeY(rng) });
			break;
		case 2:
			wld.AddPlayerWithTag<HashStringManager::Enemy03>({ rangeX(rng),rangeY(rng) });
			break;
		}
		//wld.AddPlayerWithTag<HashStringManager::Enemy01>({ rangeX(rng),rangeY(rng) });
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
	camSystem.Update(wnd.window, wnd.kbd, dt);
	wld.Update(dt, wnd.kbd, wnd.mouse);
}

void Game::ComposeFrame()
{
	wld.Draw(gfx);
}
