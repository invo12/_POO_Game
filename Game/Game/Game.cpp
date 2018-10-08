#include "Game.h"
#include "TextureManager.h"
#include "Player.h"
using namespace std;

Player* player = nullptr;

SDL_Renderer* Game::renderer = nullptr;
Map* map;
int speed = 3;
Tile* tile = nullptr;
//constructor
Game::Game()
{}

//deconstructor
Game::~Game()
{}

void Game::Init(const char* title, int xPosition, int yPosition, int width, int height, bool fullScreen)
{
	int flags = fullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN;

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		cout << "System is initialized"<<endl;
		window = SDL_CreateWindow(title, xPosition, yPosition, width, height, flags);
		if (window)
		{
			cout << "window created" << endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
			cout << "Renderer created";
		}

		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	player = new Player("Assets/circle.png",64,64);
	map = new Map();
	/*for (int i = 0; i < map->totalMapCollisionTiles; ++i)
	{
		player->collisionMap[i] = map->collisionTiles[i];
	}*/
}

void Game::HandleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);	//gets the most recent event that happened in our game(mouse move,key press etc)
	int x = 0, y = 0;
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	}
	player->HandleEvents(event);
}

void Game::Update()
{
	player->Update();
}

void Game::Render()
{
	SDL_RenderClear(renderer);
	map->DrawMap();
	player->Render();
	//add stuff to render
	SDL_RenderPresent(renderer);
}

void Game::Clean()
{
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	SDL_Quit();
	cout << "Game finished";
}