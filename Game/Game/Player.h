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

	//method for placing the bomb
	void PlaceBomb();

	//array of bombs
	static Bomb* bomb[20];
	static int totalNumberOfBombs;
	
	//has player died yet?
	bool die = false;
	
	//codes for keys for customization
	unsigned int upKey;
	unsigned int downKey;
	unsigned int leftKey;
	unsigned int rightKey;
	unsigned int bombKey;

	static int numberOfPlayers;

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
	int maxNumberOfBombs;
	//keep track of all the players in the game
	static Player* players[2];

	//the collisions that should be ignored by the player
	int numberOfIgnoredCollisions;
	list<SDL_Rect> ignoredCollisions{};
	int playerNumber;
};

//method for getting rid of the player when he dies
void DeletePlayer(Player* &player);