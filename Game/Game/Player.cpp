#include "Player.h"
#include "GameConstants.h"
#include "TextureManager.h"
#include "Collision.h"
#include <iostream>

using namespace std;

Player::Player(const char* playerTextureName,int xPos,int yPos)
{
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

	//initialize the player number of bombs
	currentNumberOfBombs = 0;
	totalNumberOfBombs = 3;

	//set the proprietes of the bomb
	mBombPower = 2;

	//get the texture dimensions from the file
	srcRect.h = 1024;
	srcRect.w = 1024;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.w = 64;
	destRect.h = 64;
}

void Player::HandleEvents(SDL_Event& event)
{
	//If a key was pressed
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
	{
		//Adjust the velocity
		switch (event.key.keysym.sym)
		{
		case SDLK_UP:
			velY -= playerVelocity;
			break;
		case SDLK_DOWN:
			velY += playerVelocity;
			break;
		case SDLK_LEFT:
			velX -= playerVelocity;
			break;
		case SDLK_RIGHT:
			velX += playerVelocity;
			break;
		}
	}
	else if (event.type == SDL_KEYUP && event.key.repeat == 0)
	{
		//Adjust the velocity
		switch (event.key.keysym.sym)
		{
		case SDLK_UP:
			velY += playerVelocity;
			break;
		case SDLK_DOWN:
			velY -= playerVelocity;
			break;
		case SDLK_LEFT:
			velX += playerVelocity;
			break;
		case SDLK_RIGHT:
			velX -= playerVelocity;
			break;
		//place a bomb if release the space key
		case SDLK_SPACE:
			PlaceBomb();
			break;
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
	if ((posX < 64) || (posX + playerWidth > (GameConstants::screenWidth - 64)) || Collision::touchCollisionTile(playerCollider,Map::collisionTiles))// || touchCollisionTile(boxCollider,test))
	{
		//don't move
		posX -= velX;
		playerCollider.x = posX;
	}
	//Move the player on Y axis and update box collider position
	posY += velY;
	playerCollider.y = posY;

	//again,keep player on the screen
	if ((posY < 64) || (posY + playerHeight > (GameConstants::screenHeight - 64)) || Collision::touchCollisionTile(playerCollider, Map::collisionTiles))// || touchCollisionTile(boxCollider, test))
	{
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
				delete bomb[i];
				bomb[i] = nullptr;
				currentNumberOfBombs--;
				cout << currentNumberOfBombs << endl;
			}
		}
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
	x = x - x % 64;

	int y = playerCollider.y + playerCollider.h / 2;
	y = y - y % 64;

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
			currentNumberOfBombs++;
		}
	}
}