#pragma once
#include "Game.h"
#include "Map.h"
#include "Bomb.h"
class Player
{
public:
	//dimensions of the player
	static const int playerWidth = GameConstants::tileWidth;
	static const int playerHeight = GameConstants::tileHeight;

	//max velocity of the player
	static const int playerVelocity = 2;

	//constructor
	Player(const char* playerTextureName,int xPos,int yPos,unsigned int upKey, unsigned int downKey, unsigned int leftKey, unsigned int rightKey,unsigned int bombKey);
	~Player();
	//Take input and adjust player movement
	void HandleEvents(SDL_Event& e);
	//move the player
	void Move();

	//update the position of the player
	void Update();

	//show the player on the screen
	void Render();

	void PlaceBomb();

	static Bomb* bomb[20];
	static int totalNumberOfBombs;
	bool die = false;
	
	unsigned int upKey;
	unsigned int downKey;
	unsigned int leftKey;
	unsigned int rightKey;
	unsigned int bombKey;

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
	int currentNumberOfBombs;

	//proprietes of the player
	int mBombPower;

	Bomb* currentBomb;
	static Player* players[2];
	static int numberOfPlayers;
};

void DeletePlayer(Player* &player);