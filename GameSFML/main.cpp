#include "MainWindow.h"
#include "Game.h"
int main()
{
	MainWindow wnd;
	Game game{ wnd };

	while (wnd.Update())
	{
		game.Go();
	}

	return 0;
}