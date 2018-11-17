#include "SDL.h"
#include "Menu.h"
#include "Game.h"

Game *game = nullptr;
Menu *menu = nullptr;

int main(int argc, char*argv[])
{
	const int FPS = 60;					//the target fps
	const int frameDelay = 1000 / FPS;	//1s/number of frames

	Uint32 frameStart;
	int frameTime;

	game = new Game();
	menu = new Menu(game);
	game->Init("AC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GameConstants::screenWidth, GameConstants::screenHeight, false);
	menu->BuildTheMenu();
	while (game->Running())
	{
		frameStart = SDL_GetTicks();
		if (!game->IsPaused())
		{
			game->HandleEvents();
			game->Update();
			game->Render();
		}
		else
		{
			menu->Update();
			menu->Render();
		}
		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}
	game->Clean();
	delete game;
	delete menu;
	return 0;
}