#pragma once
#include<iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "Tile.h"
#include "Collision.h"
class Game
{
public:
	Game();	//what happens when u start the game
	~Game();//what happens when you close the game

	void Init(const char* title, int xPosition, int yPosition, int width, int height, bool fullScreen);
	
	void HandleEvents();	//handling events
	void Update();			//update positions etc
	void Render();			//rendering method
	void Clean();			//Clear the desktop

	bool Running() { return isRunning; }
	static SDL_Renderer *renderer;

private:
	int count;
	bool isRunning;
	SDL_Window *window;
};

