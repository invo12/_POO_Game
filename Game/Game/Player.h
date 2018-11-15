#pragma once
#include "Game.h"
#include "Map.h"
#include "Bomb.h"
#include "PowerUp.h"
class Player
{
public:

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

	//keep track of all the players in the game
	static int numberOfPlayers;
	static Player* players[2];

	//get the collider of the player
	SDL_Rect GetCollider();

	//increase stats of the player
	void IncreaseNumberOfBombs();
	void IncreaseFire();
	void IncreaseSpeed();

	void SaveSpeed();
	void LoadSpeed();
	//BOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM
	int currentNumberOfBombs;

	bool isDead();
private:
	//positions of the player
	float posX,posY;

	//The velocity of the player
	int velX,velY;

	//collider box
	SDL_Rect playerCollider;
	
	//rendering part
	SDL_Texture* playerTexture;
	SDL_Rect srcRect, destRect;

	//proprietes of the player
	int mBombPower;
	int maxNumberOfBombs;

	float playerVelocity;

	//the collisions that should be ignored by the player
	int numberOfIgnoredCollisions;
	list<SDL_Rect> ignoredCollisions{};
	int playerNumber;

	//method for placing the bomb
	void PlaceBomb();

	//codes for keys for customization
	unsigned int upKey;
	unsigned int downKey;
	unsigned int leftKey;
	unsigned int rightKey;
	unsigned int bombKey;

	//has player died yet?
	bool die = false;
	bool bUp, bDown, bLeft, bRight;

	int savedSpeedX = 0;
	int savedSpeedY = 0;
};

//method for getting rid of the player when he dies
void DeletePlayer(Player* &player);