#include "Player.h"
#include "GameConstants.h"
#include "TextureManager.h"
#include "Collision.h"
#include <math.h>
#include <iostream>
#include <list>

using namespace std;

int Player::numberOfPlayers = 0;
Player* Player::players[2] = {nullptr};

Player::Player(const char* playerTextureName,int xPos,int yPos,unsigned int upKey, unsigned int downKey, unsigned int leftKey, unsigned int rightKey,unsigned int bombKey)
{
	players[numberOfPlayers] = this;
	numberOfPlayers++;

	//load the texture on the player
	playerTexture = TextureManager::LoadTexture(playerTextureName);
	
	//set the collider
	playerCollider.w = GameConstants::playerWidth;
	playerCollider.h = GameConstants::playerHeight;

	//Initialize the offsets
	posX = xPos;
	posY = yPos;

	//initialize velocities
	velX = 0;
	velY = 0;
	
	bUp = false;
	bDown = false;
	bLeft = false;
	bRight = false;

	//PROPRIETEEEES
	//initialize the player number of bombs
	currentNumberOfBombs = 0;
	maxNumberOfBombs = 1;
	mBombPower = 2;

	//get the texture dimensions from the file
	srcRect.h = 1024;
	srcRect.w = 1024;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.w = GameConstants::playerWidth;
	destRect.h = GameConstants::playerHeight;

	this->upKey = upKey;
	this->downKey = downKey;
	this->leftKey = leftKey;
	this->rightKey = rightKey;
	this->bombKey = bombKey;

	playerNumber = numberOfPlayers-1;
	playerVelocity = 2;
}

Player::~Player()
{
	cout << "PlayerDIEEED";
	numberOfPlayers--;
}

void Player::HandleEvents(SDL_Event& event)
{
	//If a key was pressed
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
	{
		//place a bomb if we press the place bomb key
		if (event.key.keysym.sym == bombKey)
		{
			PlaceBomb();
		}
		//Adjust the velocity
		if (event.key.keysym.sym == upKey)
		{
			velY -= playerVelocity;
			bUp = true;
		}
		else if (event.key.keysym.sym == downKey)
		{
			velY += playerVelocity;
			bDown = true;
		}
		else if (event.key.keysym.sym == leftKey)
		{
			velX -= playerVelocity;
			bLeft = true;
		}
		else if (event.key.keysym.sym == rightKey)
		{
			velX += playerVelocity;
			bRight = true;
		}
	}
	else if (event.type == SDL_KEYUP && event.key.repeat == 0)
	{
		//Adjust the velocity
		if (event.key.keysym.sym == upKey && bUp)
		{
			velY += playerVelocity;
		}
		if (event.key.keysym.sym == downKey && bDown)
		{
			velY -= playerVelocity;
		}
		if (event.key.keysym.sym == leftKey && bLeft)
		{
			velX += playerVelocity;
		}
		if (event.key.keysym.sym == rightKey && bRight)
		{
			velX -= playerVelocity;
		}
	}
	Move();
}

void Player::Move()
{
	//Move the player on X axis and update box collider position
	posX += velX;
	playerCollider.x = posX;

	//keep player on the screen
	if ((posX < GameConstants::tileWidth) || (posX + GameConstants::playerWidth > (GameConstants::screenWidth - GameConstants::tileWidth))
		|| Collision::touchCollisionTile(playerCollider, Map::collisionTiles)
		|| Collision::touchCollisionTile(playerCollider, Map::bombTiles, ignoredCollisions))// || touchCollisionTile(boxCollider,test))
	{
		//don't move
		posX -= velX;
		playerCollider.x = posX;
	}

	//Move the player on Y axis and update box collider position
	posY += velY;
	playerCollider.y = posY;

	//again,keep player on the screen
	if ((posY < GameConstants::tileHeight) || (posY + GameConstants::playerHeight > (GameConstants::screenHeight - GameConstants::tileHeight)) 
		|| Collision::touchCollisionTile(playerCollider, Map::collisionTiles) 
		|| Collision::touchCollisionTile(playerCollider,Map::bombTiles,ignoredCollisions))// || touchCollisionTile(boxCollider, test))
	{
		//if (Map::GetTileType(posX, posY) == TileType::EXPLOSION)
		//	die = true;
		//go back to initial position 
		posY -= velY;
		playerCollider.y = posY;
	}
}

void Player::Update()
{
	//move the player in a certain direction
	destRect.x = posX;
	destRect.y = posY;
	list<Bomb*>::iterator bombIterator;

	for (bombIterator = Game::bombs.begin(); bombIterator != Game::bombs.end(); ++bombIterator)
	{
		if ((*bombIterator)->set[playerNumber] == 0 && abs(posX - (*bombIterator)->GetBomb().x) < 60 && abs(posY - (*bombIterator)->GetBomb().y) < 60)
		{
			(*bombIterator)->set[playerNumber] = 1;
			ignoredCollisions.push_front((*bombIterator)->GetBomb());
		}
		if ((*bombIterator)->set[playerNumber] != 2 && ((abs((*bombIterator)->GetBomb().x - posX) > GameConstants::tileWidth - 1) || (abs((*bombIterator)->GetBomb().y - posY) > GameConstants::tileHeight - 1)))
		{
			list<SDL_Rect>::iterator it;
			for (it = ignoredCollisions.begin(); it != ignoredCollisions.end(); ++it)
			{
				if (SDL_RectEquals(&(*it), &(*bombIterator)->GetBomb()))
				{
					ignoredCollisions.erase(it);
					break;
				}
			}
			(*bombIterator)->set[playerNumber] = 2;
		}
	}

	if (Collision::touchCollisionTile(playerCollider, Map::explosionTiles))
	{
		die = true;
	}
}

void Player::Render()
{
	//render the player
	SDL_RenderCopy(Game::renderer, playerTexture, &srcRect, &destRect);
}

void Player::PlaceBomb()
{
	//we have totalNumberOfBombs maximum and we must insert one to the index,
	int index = -1;

	//calculate the center of the player
	//and place the bomb acordingly
	int x = playerCollider.x + playerCollider.w / 2;
	x = x - x % GameConstants::tileWidth;

	int y = playerCollider.y + playerCollider.h / 2;
	y = y - y % GameConstants::tileHeight;

	list<Bomb*>::iterator bombIterator;
	//if we haven't already placed all bombs
	if (currentNumberOfBombs < maxNumberOfBombs)
	{
		for (bombIterator = Game::bombs.begin(); bombIterator != Game::bombs.end(); ++bombIterator)
		{
			//check if a bomb it's not already there
			if ((*bombIterator)->GetBomb().x == x && (*bombIterator)->GetBomb().y == y)
				return;
		}
		//if we find an empty place or a bomb it's not already there place the bomb
		currentNumberOfBombs++;
		Game::totalNumberOfBombs++;

		Game::bombs.push_front(new Bomb(x, y, mBombPower,playerNumber));
		Map::SetTileType(x, y, TileType::BOMB);

		for (int i = 0; i < numberOfPlayers; ++i)
		{
			(*Game::bombs.begin())->set[i] = 0;
		}
		Collision::AddCollisionOnMap(Map::bombTiles, x, y, TileType::BOMB);

	}
}

SDL_Rect Player::GetCollider()
{
	return playerCollider;
}

void DeletePlayer(Player* &player)
{
	delete player;
	player = nullptr;
}

void Player::IncreaseNumberOfBombs()
{
	maxNumberOfBombs++;
}

void Player::IncreaseFire()
{
	mBombPower++;
}

void Player::IncreaseSpeed()
{
	float newPlayerVelocity = playerVelocity + 0.5f;
	if (velX == (int)playerVelocity)
	{
		velX = newPlayerVelocity;
	}
	else if (velX == -(int)playerVelocity)
	{
		velX = -newPlayerVelocity;
	}
	if (velY == (int)playerVelocity)
	{
		velY = newPlayerVelocity;
	}
	else if (velY == -(int)playerVelocity)
	{
		velY = -newPlayerVelocity;
	}
	playerVelocity = newPlayerVelocity;
}

bool Player::isDead()
{	
	return die;
}