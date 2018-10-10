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
		dest.x = xPos;
		dest.y = yPos;
		TextureManager::Draw(mExplosionTexture,src,dest);
		for (int i = 0; i < mMaxUp; ++i)
		{
			dest.y -= 64;
			TextureManager::Draw(mExplosionUpTexture, src, dest);
		}
		dest.x = xPos;
		dest.y = yPos;
		for (int i = 0; i < mMaxDown; ++i)
		{
			dest.y += 64;
			TextureManager::Draw(mExplosionUpTexture, src, dest);
		}
		dest.x = xPos;
		dest.y = yPos;
		for (int i = 0; i < mMaxRight; ++i)
		{
			dest.x += 64;
			TextureManager::Draw(mExplosionRightTexture, src, dest);
		}
		dest.x = xPos;
		dest.y = yPos;
		for (int i = 0; i < mMaxLeft; ++i)
		{
			dest.x -= 64;
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
	for (int i = 0; i < mBombPower; ++i)
	{
		temp = Map::GetTileType(xPos, yPos - (i + 1) * 64);
		if (temp == 0)
		{
			mMaxUp++;
		}
		else if (temp == 1)
		{
			break;
		}
		else if (temp == 2)
		{
			mMaxUp++;
			break;
		}
	}
	
	//down
	for (int i = 0; i < mBombPower; ++i)
	{
		temp = Map::GetTileType(xPos, yPos + (i + 1) * 64);
		if (temp == 0)
		{
			mMaxDown++;
		}
		else if (temp == 1)
		{
			break;
		}
		else if (temp == 2)
		{
			mMaxDown++;
			break;
		}
	}
	
	//left
	for (int i = 0; i < mBombPower; ++i)
	{
		temp = Map::GetTileType(xPos - (i+1)*64, yPos);
		if (temp == 0)
		{
			mMaxLeft++;
		}
		else if (temp == 1)
		{
			break;
		}
		else if (temp == 2)
		{
			mMaxLeft++;
			break;
		}
	}
	
	//right
	for (int i = 0; i < mBombPower; ++i)
	{
		temp = Map::GetTileType(xPos + (i+1) * 64, yPos);
		if (temp == 0)
		{
			mMaxRight++;
		}
		else if (temp == 1)
		{
			break;
		}
		else if (temp == 2)
		{
			mMaxRight++;
			break;
		}
	}
	std::cout << mMaxUp << ' ' << mMaxRight << ' ' << mMaxDown << ' ' << mMaxLeft << std::endl;
}