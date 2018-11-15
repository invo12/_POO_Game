#include "Game.h"
#include "TextureManager.h"
#include "Player.h"
#include "Collision.h"
#include "Menu.h"

using namespace std;

Player* players[2]{ nullptr };

SDL_Renderer* Game::renderer = nullptr;
Map* map;
Uint32 Game::pauseTime;
list<Bomb*> Game::bombs{};
int Game::totalNumberOfBombs;
list<PowerUp*> Game::powerUps{};

//constructor
Game::Game()
{}

//deconstructor
Game::~Game()
{

}

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
		Pause();
	}
	else
	{
		isRunning = false;
	}

	InitOthers();
	Bomb::Init();
	PowerUp::Init();
}

void Game::HandleEvents()
{
	const Uint8 *keys = SDL_GetKeyboardState(nullptr);
	SDL_Event event;
	SDL_PollEvent(&event);	//gets the most recent event that happened in our game(mouse move,key press etc)
	SDL_PumpEvents();
	int x = 0, y = 0;
	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			Pause();
		}
		break;
	}
	for (int i = 0; i < 2; ++i)
	{
		if (players[i] != nullptr)
			players[i]->HandleEvents(keys);
	}
	//exitButton->Update(event);
}

void Game::Update()
{
	if (players[0] == nullptr && players[1] == nullptr)
	{
		ClearTheMap();
		InitOthers();
		return;
	}
	for (int i = 0; i < 2; ++i)
	{
		if (players[i] != nullptr)
		{
			players[i]->Update();
			if (players[i]->isDead())
				DeletePlayer(players[i]);
			if (Player::numberOfPlayers == 1)
			{
				cout << "Player " << i << " won";
				ClearTheMap();
				InitOthers();
			}
		}
	}
	//bombs
	list<Bomb*>::iterator bombIterator;
	for (bombIterator = bombs.begin(); bombIterator != bombs.end(); ++bombIterator)
	{
		if (!(*bombIterator)->Update())
		{
			for (int i = 0; i < 2; ++i)
			{
				if ((*bombIterator)->GetParent() == i && players[i] != nullptr)
				{
					players[i]->currentNumberOfBombs--;
				}
			}
			Collision::RemoveCollisionFromMap(map->bombTiles, (*bombIterator)->GetBomb().x, (*bombIterator)->GetBomb().y);
			delete *bombIterator;
			bombs.remove(*bombIterator);
			break;
		}
	}

	//the powerUps
	list<PowerUp*>::iterator powerUpIterator;
	for (powerUpIterator = Game::powerUps.begin(); powerUpIterator != Game::powerUps.end(); ++powerUpIterator)
	{
		int destroy = (*powerUpIterator)->Update();
		if (destroy != -1)
		{
			if ((*powerUpIterator)->mType == PowerUpType::BOMB)
			{
				players[destroy]->IncreaseNumberOfBombs();
			}
			else if ((*powerUpIterator)->mType == PowerUpType::FIRE)
			{
				players[destroy]->IncreaseFire();
			}
			else if ((*powerUpIterator)->mType == PowerUpType::SPEED)
			{
				players[destroy]->IncreaseSpeed();
			}
			delete (*powerUpIterator);
			Game::powerUps.erase(powerUpIterator);
			break;
		}
	}
}

void Game::Render()
{
	//clear the render
	SDL_RenderClear(renderer);

	//then render the map
	map->DrawMap();
	//then the player
	for (int i = 0; i < 2; ++i)
		if (players[i] != nullptr)
		{
			players[i]->Render();
		}
	//bombs
	list<Bomb*>::iterator bombIterator;
	for (bombIterator = bombs.begin(); bombIterator != bombs.end(); ++bombIterator)
	{
		(*bombIterator)->Render();
	}
	//the powerUps
	list<PowerUp*>::iterator powerUpIterator;
	for (powerUpIterator = powerUps.begin(); powerUpIterator != powerUps.end(); ++powerUpIterator)
	{
		(*powerUpIterator)->Render();
	}
	//and show all the results to the screen
	SDL_RenderPresent(renderer);
}

void Game::Stop()
{
	isRunning = false;
}

void Game::Clean()
{
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	delete map;
	map = nullptr;
	ClearLists();
	SDL_Quit();
	cout << "Game finished";
}

void Game::ClearLists()
{
	if (!bombs.empty())
	{
		list<Bomb*>::iterator it;
		for (it = bombs.begin(); it != bombs.end(); ++it)
		{
			delete(*it);
			*it = nullptr;
		}
		bombs.clear();
	}
	if (!powerUps.empty())
	{
		list<PowerUp*>::iterator it;
		for (it = powerUps.begin(); it != powerUps.end(); ++it)
		{
			delete *it;
			*it = nullptr;
		}
		powerUps.clear();
	}
}

void InitOthers()
{
	players[0] = new Player("Assets/player1.png", GameConstants::tileWidth + 5, GameConstants::tileHeight + 5, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_SPACE);
	players[1] = new Player("Assets/player2.png",GameConstants::screenWidth - 2 * GameConstants::tileWidth - 5,GameConstants::screenHeight - 2 * GameConstants::tileHeight + 5, SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D, SDL_SCANCODE_Q);
	map = new Map();
}

void Game::ClearTheMap()
{
	delete map;
	map = nullptr;
	ClearLists();
	for (int i = 0; i < 2; ++i)
	{
		if (players[i] != nullptr)
			DeletePlayer(players[i]);
	}
}

void Game::Resume()
{
	onPause = false;
	for (auto it : bombs)
	{
		it->ResumeTimer();
	}
}