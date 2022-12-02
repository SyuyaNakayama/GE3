#include "MyGame.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	MyGame game;
	game.Initialize();

	while (true)
	{
		game.Update();
		if (game.IsEndRequest()) { break; }
		game.Draw();
	}

	game.Finalize();

	return 0;
}