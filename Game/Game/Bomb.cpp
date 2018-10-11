#include "Bomb.h"
#include "Map.h"
#include <iostream>

Bomb::Bomb(int x,int y,int bombPower)
{
	//load the texture
	mBombTexture = TextureManager::LoadTexture("Assets/bomb.png");
	mExplosionTexture = TextureManager::LoadTexture("Assets/explosion.png");
	mExplosionUpTexture = TextureManager::LoadTexture("Assets/explosion_up.png");
	mExplosionRightTexture = TextureManager::LoadTexture("Assets/explosion_right.png");

	//cut the bomb from file
	src.x = src.y = 0;
	src.w = 300;
	src.h = 300;

	//place the bomb on the specific tile
	dest.w = GameConstants::tileWidth;
	dest.h = GameConstants::tileHeight;
	dest.x = x;
	dest.y = y;

	//start the bomb timer
	startTime = SDL_GetTicks();
	
	//proprietes of the bomb
	mBombPower = bombPower;

	xPos = x;
	yPos = y;
}

void Bomb::Update()
{
	//if time has run out then explode
	if (!explode && SDL_GetTicks() - startTime > mTimeToExplode)
	{
		explode = true;
		Explode();
	}
	else if(explode && SDL_GetTicks() - startTime > mTimeToExplode + mTimeToDestroyAfterExplosion)
	{
		destroy = true;
	}
}

void Bomb::Render()
{
	if (!explode)
	{
		TextureManager::Draw(mBombTexture, src, dest);
	}
	else
	{
		//up
		setDestination(dest, xPos, yPos);
		TextureManager::Draw(mExplosionTexture,src,dest);
		for (int i = 0; i < mMaxUp; ++i)
		{
			dest.y -= GameConstants::tileHeight;
			TextureManager::Draw(mExplosionUpTexture, src, dest);
		}

		//down
		setDestination(dest, xPos, yPos);
		for (int i = 0; i < mMaxDown; ++i)
		{
			dest.y += GameConstants::tileHeight;
			TextureManager::Draw(mExplosionUpTexture, src, dest);
		}

		//right
		setDestination(dest, xPos, yPos);
		for (int i = 0; i < mMaxRight; ++i)
		{
			dest.x += GameConstants::tileWidth;
			TextureManager::Draw(mExplosionRightTexture, src, dest);
		}

		//left
		setDestination(dest, xPos, yPos);
		for (int i = 0; i < mMaxLeft; ++i)
		{
			dest.x -= GameConstants::tileWidth;
			TextureManager::Draw(mExplosionRightTexture, src, dest);
		}
	}
}

SDL_Rect Bomb::GetBomb()
{
	return dest;
}

Bomb::~Bomb()
{
	//std::cout << "Bomb exploded";
}

void Bomb::Explode()
{
	int xPos = dest.x;
	int yPos = dest.y;
	int temp;
	//up
	GetMaxDistanceInDirection(mMaxUp, mBombPower, xPos, yPos, 0, -1);
	
	//down
	GetMaxDistanceInDirection(mMaxDown, mBombPower, xPos, yPos, 0, 1);
	
	//left
	GetMaxDistanceInDirection(mMaxLeft, mBombPower, xPos, yPos, -1, 0);
	
	//right
	GetMaxDistanceInDirection(mMaxRight, mBombPower, xPos, yPos, 1, 0);
}

void setDestination(SDL_Rect &dest,int x,int y)
{
	dest.x = x;
	dest.y = y;
}

void GetMaxDistanceInDirection(int &maxDistance,int bombPower,int xPos,int yPos,int xSign,int ySign)
{
	int temp = 0;
	for (int i = 0; i < bombPower; ++i)
	{
		temp = Map::GetTileType(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i+1) * GameConstants::tileHeight * ySign);
		if (temp == 0)
		{
			maxDistance++;
		}
		else if (temp == 1)
		{
			break;
		}
		else if (temp == 2)
		{
			maxDistance++;
			Map::DestroyBlock(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign);
			break;
		}
	}
}