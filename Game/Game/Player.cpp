#include "Player.h"
#include "GameConstants.h"
#include "TextureManager.h"
#include "Collision.h"
#include <math.h>
#include <iostream>

using namespace std;

Bomb* Player::bomb[20] = { nullptr };
int Player::totalNumberOfBombs;
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
	totalNumberOfBombs = 3;
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
		|| (!Bomb::notSolidBombs.empty() && Collision::touchCollisionTile(playerCollider,Bomb::notSolidBombs,currentBomb == nullptr ? nullptr:&currentBomb->GetBomb())))// || touchCollisionTile(boxCollider,test))
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
		|| (!Bomb::notSolidBombs.empty() && Collision::touchCollisionTile(playerCollider, Bomb::notSolidBombs, currentBomb == nullptr ? nullptr : &currentBomb->GetBomb())))// || touchCollisionTile(boxCollider, test))
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
			if (bomb[i]->destroy)
			{
				if (bomb[i]->solid)
				{
					Collision::RemoveCollisionFromMap(Map::collisionTiles, bomb[i]->GetBombCollider().x, bomb[i]->GetBombCollider().y);
				}
				delete bomb[i];
				bomb[i] = nullptr;
				currentNumberOfBombs--;
			}
		}
	}
	if (currentBomb != nullptr && ((abs(currentBomb->GetBomb().x - posX) > GameConstants::tileWidth - 4) || (abs(currentBomb->GetBomb().y - posY) > GameConstants::tileHeight - 4)))
	{
		bool found = false;
		for (int j = 0; j < numberOfPlayers; ++j)
		{
			if (players[j] != this && ((abs(currentBomb->GetBomb().x - players[j]->posX) < GameConstants::tileWidth - 4) && (abs(currentBomb->GetBomb().y - players[j]->posY) < GameConstants::tileHeight - 4)))
			{
				cout << abs(currentBomb->GetBomb().x - players[j]->posX) << ' ' << abs(currentBomb->GetBomb().y - players[j]->posY) << endl;
				players[j]->currentBomb = currentBomb;
				found = true;
			}
		}
		if (!found)
		{
			RemoveSolidBombFromList(currentBomb->GetBomb());
			currentBomb->MakeBombSolid();
			Collision::AddCollisionOnMap(Map::collisionTiles, currentBomb->GetBombCollider().x, currentBomb->GetBombCollider().y, TileType::BOMB);
		}
		currentBomb = nullptr;
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

	//if we haven't already placed all bombs
	if (currentNumberOfBombs < totalNumberOfBombs)
	{
		for (int i = 0; i < totalNumberOfBombs; ++i)
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
			bomb[index] = new Bomb(x, y,mBombPower);
			Map::SetTileType(x, y, TileType::BOMB);
			currentNumberOfBombs++;
			currentBomb = bomb[index];
			AddNotSolidBombOnList(currentBomb->GetBomb());
		}
	}
}

void DeletePlayer(Player* &player)
{
	delete player;
	player = nullptr;
}