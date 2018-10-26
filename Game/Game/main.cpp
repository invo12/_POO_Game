#include "SDL.h"

#include "Game.h"
#include "GameConstants.h"

Game *game = nullptr;

int main(int argc, char*argv[])
{
	const int FPS = 60;					//the target fps
	const int frameDelay = 1000 / FPS;	//1s/number of frames

	Uint32 frameStart;
	int frameTime;

	game = new Game();
	game->Init("AC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GameConstants::screenWidth, GameConstants::screenHeight, false);
	while (game->Running())
	{
		frameStart = SDL_GetTicks();

		game->HandleEvents();
		game->Update();
		game->Render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}
	//system("color 2");
	game->Clean();
	return 0;
}