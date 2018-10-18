#include "Player.h"
#include "GameConstants.h"
#include "TextureManager.h"
#include "Collision.h"
#include <math.h>
#include <iostream>
#include <list>

using namespace std;

Bomb* Player::bomb[20] = { nullptr };
int Player::totalNumberOfBombs = 0;
int Player::numberOfPlayers = 0;
Player* Player::players[2] = {nullptr};

Player::Player(const char* playerTextureName,int xPos,int yPos,unsigned int upKey, unsigned int downKey, unsigned int leftKey, unsigned int rightKey,unsigned int bombKey)
{
	players[numberOfPlayers] = this;
	numberOfPlayers++;

	//load the texture on the player
	playerTexture = TextureManager::LoadTexture(playerTextureName);
	
	//set the collider
	playerCollider.w = playerWidth;
	playerCollider.h = playerHeight;

	//Initialize the offsets
	posX = xPos;
	posY = yPos;

	//Initialize the velocity
	velX = 0;
	velY = 0;

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

	destRect.w = 64;
	destRect.h = 64;

	this->upKey = upKey;
	this->downKey = downKey;
	this->leftKey = leftKey;
	this->rightKey = rightKey;
	this->bombKey = bombKey;

	playerNumber = numberOfPlayers-1;
	cout << playerNumber;
}

Player::~Player()
{
	cout << "PlayerDIEEED";
}

void Player::HandleEvents(SDL_Event& event)
{
	//If a key was pressed
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
	{
		//Adjust the velocity
		if (event.key.keysym.sym == upKey)
		{
			velY -= playerVelocity;
		}
		else if (event.key.keysym.sym == downKey)
		{
			velY += playerVelocity;
		}
		else if (event.key.keysym.sym == leftKey)
		{
			velX -= playerVelocity;
		}
		else if (event.key.keysym.sym == rightKey)
		{
			velX += playerVelocity;
		}
	}
	else if (event.type == SDL_KEYUP && event.key.repeat == 0)
	{
		//Adjust the velocity
		if (event.key.keysym.sym == upKey)
		{
			velY += playerVelocity;
		}
		if (event.key.keysym.sym == downKey)
		{
			velY -= playerVelocity;
		}
		if (event.key.keysym.sym == leftKey)
		{
			velX += playerVelocity;
		}
		if (event.key.keysym.sym == rightKey)
		{
			velX -= playerVelocity;
		}
		//place a bomb if release the space key
		if (event.key.keysym.sym == bombKey)
		{
			PlaceBomb();
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
	if ((posX < GameConstants::tileWidth) || (posX + playerWidth > (GameConstants::screenWidth - GameConstants::tileWidth))
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
	if ((posY < GameConstants::tileHeight) || (posY + playerHeight > (GameConstants::screenHeight - GameConstants::tileHeight)) 
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

	for (int i = 0; i < totalNumberOfBombs; ++i)
	{
		if (bomb[i] != nullptr)
		{
			bomb[i]->Update();

			if (bomb[i]->set[playerNumber] == 0 && abs(players[playerNumber]->posX - bomb[i]->GetBomb().x) < 60 && abs(players[playerNumber]->posY - bomb[i]->GetBomb().y) < 60)
			{
				bomb[i]->set[playerNumber] = 1;
				ignoredCollisions.push_front(bomb[i]->GetBomb());
			}
			if (bomb[i]->set[playerNumber] != 2 && ((abs(bomb[i]->GetBomb().x - players[playerNumber]->posX) > GameConstants::tileWidth - 1) || (abs(bomb[i]->GetBomb().y - players[playerNumber]->posY) > GameConstants::tileHeight - 1)))
			{
				if (!players[playerNumber]->ignoredCollisions.empty())
				{
					list<SDL_Rect>::iterator it;
					for (it = ignoredCollisions.begin(); it != ignoredCollisions.end(); ++it)
					{
						if (SDL_RectEquals(&(*it), &bomb[i]->GetBomb()))
						{
							cout << "removed\n";
							ignoredCollisions.erase(it);
							break;
						}
					}
					bomb[i]->set[playerNumber] = 2;
				}
			}

			if (bomb[i]->destroy)
			{
				Collision::RemoveCollisionFromMap(Map::bombTiles, bomb[i]->GetBomb().x, bomb[i]->GetBomb().y);
				delete bomb[i];
				bomb[i] = nullptr;
				currentNumberOfBombs--;
			}
		}
	}
	if (Collision::touchCollisionTile(playerCollider, Map::explosionTiles))
	{
		die = true;
	}
}

void Player::Render()
{
	//render each active bomb
	for (int i = 0; i < totalNumberOfBombs; ++i)
	{
		if (bomb[i] != nullptr)
		{
			bomb[i]->Render();
		}
	}
	//and the player
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

	cout << currentNumberOfBombs << ' ';
	//if we haven't already placed all bombs
	if (currentNumberOfBombs < maxNumberOfBombs)
	{
		for (int i = 0; i < maxNumberOfBombs; ++i)
		{
			//check if it's an empty place
			if (index == -1 && bomb[i] == nullptr)
			{
				index = i;
			}
			else if(bomb[i] != nullptr)
			{
				//check if a bomb it's not already there
				if (bomb[i]->GetBomb().x == x && bomb[i]->GetBomb().y == y)
					return;
			}
			
		}
		//if we find an empty place or a bomb it's not already there place the bomb
		if (index != -1)
		{
			currentNumberOfBombs++;
			totalNumberOfBombs++;
			bomb[index] = new Bomb(x, y,mBombPower);
			Map::SetTileType(x, y, TileType::BOMB);
			for (int i = 0; i < numberOfPlayers; ++i)
			{
				bomb[index]->set[i] = 0;
			}
			Collision::AddCollisionOnMap(Map::bombTiles, bomb[index]->GetBomb().x, bomb[index]->GetBomb().y, TileType::BOMB);
		}
	}
}

void DeletePlayer(Player* &player)
{
	delete player;
	Player::numberOfPlayers--;
	player = nullptr;
}