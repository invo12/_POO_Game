#pragma once
#include<iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "Tile.h"
#include "Collision.h"
#include "GameConstants.h"
#include "PowerUp.h"
#include "Bomb.h"
#include <list>

class Game
{
public:
	Game();	//what happens when u start the game
	~Game();//what happens when you close the game

	void Init(const char* title, int xPosition, int yPosition, int width, int height, bool fullScreen);
	
	void HandleEvents();	//handling events
	void Update();			//update positions etc
	void Render();			//rendering method
	void Stop();
	void Clean();			//Clear the desktop

	inline bool Running() { return isRunning; }	//is game still running?
	inline bool IsPaused() { return onPause; }
	void Pause();
	void Resume();
	static SDL_Renderer *renderer;			//renderer

	static Uint32 pauseTime;
	static int totalNumberOfBombs;
	static list<Bomb*> bombs;
	static list<PowerUp*> powerUps;
private:		
	bool isRunning;
	bool onPause;
	SDL_Window *window;
	void ClearLists();
	void ClearTheMap();
};

void InitOthers();