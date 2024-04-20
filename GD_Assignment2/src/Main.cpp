#include "Game.hpp"

int main()
{
	uint32_t wWidth = 1280;
	uint32_t wHeight = 720;

	Game game(wWidth, wHeight);

	while (game.isRunning()) {
		game.update();
	}

	return 0;
}
