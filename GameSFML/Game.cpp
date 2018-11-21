#include "Game.h"
#include "MainWindow.h"
Game::Game(MainWindow & wnd)
	:
	wnd(wnd),
	gfx(wnd.window)
{
	std::uniform_real_distribution<float> rangeX(-20.0f, 20.0f);
	std::uniform_real_distribution<float> rangeY(-20.0f, 20.0f);
	std::uniform_int_distribution<int> range(0, 3);
	/*for (size_t i = 0; i < 50; i++)
	{
		switch (range(rng))
		{
		case 0:
			wld.AddPlayerWithTag({ rangeX(rng),rangeY(rng) }, HS::Enemy01);
			break;
		case 1:
			wld.AddPlayerWithTag({ rangeX(rng),rangeY(rng) }, HS::Enemy02);
			break;
		case 2:
			wld.AddPlayerWithTag({ rangeX(rng),rangeY(rng) }, HS::Enemy03);
			break;
		case 3:
			wld.AddPlayerWithTag({ rangeX(rng),rangeY(rng) }, HS::Enemy04);
			break;
		}
		
	}*/
	wld.AddPlayer({ rangeX(rng),rangeY(rng) });
	
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
