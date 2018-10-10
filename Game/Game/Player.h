#pragma once
#include "Game.h"
#include "Map.h"
#include "Bomb.h"
class Player
{
public:
	//dimensions of the player
	static const int playerWidth = 64;
	static const int playerHeight = 64;

	//max velocity of the player
	static const int playerVelocity = 2;

	//constructor
	Player(const char* playerTextureName, int xPos, int yPos);

	//Take input and adjust player movement
	void HandleEvents(SDL_Event& e);
	//move the player
	void Move();

	//update the position of the player
	void Update();

	//show the player on the screen
	void Render();

	void PlaceBomb();

private:
	//positions of the player
	int posX,posY;

	//The velocity of the player
	int velX,velY;

	//collider box
	SDL_Rect playerCollider;
	
	//rendering part
	SDL_Texture* playerTexture;
	SDL_Rect srcRect, destRect;

	//BOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM
	Bomb* bomb[10];
	int currentNumberOfBombs;
	int totalNumberOfBombs;

	int mBombPower;

};