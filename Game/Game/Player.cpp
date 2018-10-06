#include "Player.h"
#include "GameConstants.h"
#include "TextureManager.h"
#include <iostream>

using namespace std;

Player::Player(const char* playerTextureName,int xPos,int yPos)
{
	playerTexture = TextureManager::LoadTexture(playerTextureName);

	//Initialize the offsets
	posX = xPos;
	posY = yPos;

	//Initialize the velocity
	velX = 0;
	velY = 0;

	//source rectangle details

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
		}
	}
	Move();
}
void Player::Move()
{
	//Move the player on X axis
	posX += velX;

	//keep player on the screen
	if ((posX < 64) || (posX + playerWidth > (GameConstants::screenWidth - 64)))
	{
		//don't move
		posX -= velX;
	}
	//Move the player on Y axis
	posY += velY;

	//again,keep player on the screen
	if ((posY < 64) || (posY + playerHeight > (GameConstants::screenHeight - 64)))
	{
		//go back to initial position 
		posY -= velY;
	}
}

void Player::Update()
{
	srcRect.h = 1024;
	srcRect.w = 1024;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.w = 64;
	destRect.h = 64;
	destRect.x = posX;
	destRect.y = posY;

	cout << destRect.x;
}

void Player::Render()
{
	SDL_RenderCopy(Game::renderer, playerTexture, &srcRect, &destRect);
}
	