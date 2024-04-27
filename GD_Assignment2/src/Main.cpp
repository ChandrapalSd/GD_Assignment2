#include "Game.hpp"

int main()
{
	uint32_t wWidth = 1280/1.2;
	uint32_t wHeight = 720/1.2;

	Game game(wWidth, wHeight);
	game.init("config.txt");
	game.run();

	return 0;
}
