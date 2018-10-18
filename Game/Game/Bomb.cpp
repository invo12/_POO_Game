#include "Bomb.h"
#include "Map.h"
#include "Player.h"
#include <iostream>
using namespace std;
SDL_Texture* Bomb::mBombTexture;
SDL_Texture* Bomb::mExplosionTexture;
SDL_Texture* Bomb::mExplosionUpTexture;
SDL_Texture* Bomb::mExplosionRightTexture;

void Bomb::Init()
{
	mBombTexture = TextureManager::LoadTexture("Assets/bomb.png");
	mExplosionTexture = TextureManager::LoadTexture("Assets/explosion.png");
	mExplosionUpTexture = TextureManager::LoadTexture("Assets/explosion_up.png");
	mExplosionRightTexture = TextureManager::LoadTexture("Assets/explosion_right.png");
}

Bomb::Bomb(int x,int y,int bombPower)
{
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

	//set the position of the bomb
	xPos = x;
	yPos = y;
}

Bomb::~Bomb()
{
	//place grass after destruction
	Map::SetTileType(xPos, yPos, TileType::GRASS);

	//up
	RemoveExplosionColliders(mMaxUp, xPos, yPos, 0, -1);

	//down
	RemoveExplosionColliders(mMaxDown, xPos, yPos, 0, 1);

	//left
	RemoveExplosionColliders(mMaxLeft, xPos, yPos, -1, 0);

	//right
	RemoveExplosionColliders(mMaxRight, xPos, yPos, 1, 0);
	
	//remove both explosion colliders and bomb
	Collision::RemoveCollisionFromMap(Map::bombTiles, xPos, yPos);
	Collision::RemoveCollisionFromMap(Map::explosionTiles, xPos, yPos);
	Player::totalNumberOfBombs--;
}

void Bomb::Update()
{
	//if time has run out then explode
	if (!explode && SDL_GetTicks() - startTime > mTimeToExplode)
	{
		explode = true;
		Explode();
	}
	//after it explodes t\wait some time before cleaning the explosion
	else if(explode && SDL_GetTicks() - startTime > mTimeToExplode + mTimeToDestroyAfterExplosion)
	{
		destroy = true;
	}
}

void Bomb::Render()
{
	//before the explosion draw a bomb
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

//get the coordinates of the bomb
SDL_Rect Bomb::GetBomb()
{
	return dest;
}

void Bomb::Explode()
{
	int xPos = dest.x;
	int yPos = dest.y;
	int temp;
	//calculate how far should go the explosion in each direction

	//up
	GetMaxDistanceInDirection(mMaxUp, mBombPower, xPos, yPos, 0, -1);
	
	//down
	GetMaxDistanceInDirection(mMaxDown, mBombPower, xPos, yPos, 0, 1);
	
	//left
	GetMaxDistanceInDirection(mMaxLeft, mBombPower, xPos, yPos, -1, 0);
	
	//right
	GetMaxDistanceInDirection(mMaxRight, mBombPower, xPos, yPos, 1, 0);

	//set each tile to be of explosion type and add it to the explosion list
	Map::SetTileType(xPos, yPos, TileType::EXPLOSION);
	Collision::AddCollisionOnMap(Map::explosionTiles, xPos, yPos, TileType::EXPLOSION);
}

//reset the dest rect position
void setDestination(SDL_Rect &dest,int x,int y)
{
	dest.x = x;
	dest.y = y;
}

//calculate how far the explosion should go in the direction
void GetMaxDistanceInDirection(int &maxDistance,int bombPower,int xPos,int yPos,int xSign,int ySign)
{
	TileType temp;
	for (int i = 0; i < bombPower; ++i)
	{
		temp = Map::GetTileType(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i+1) * GameConstants::tileHeight * ySign);
		//if it's grass ignore it and continue
		if (temp == TileType::GRASS)
		{
			maxDistance++;
			Map::SetTileType(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign, TileType::EXPLOSION);
			Collision::AddCollisionOnMap(Map::explosionTiles, xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign, TileType::EXPLOSION);
		}
		//if it's wall then you can't pass
		else if (temp == TileType::WALL)
		{
			break;
		}
		//if it's a destroyable block then make the explosion hover it then stop 
		else if (temp == TileType::DESTROYABLEBLOCK)
		{
			Map::SetTileType(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign, TileType::EXPLOSION);
			maxDistance++;
			Map::DestroyBlock(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign);
			break;
		}
		//if it's other bomb it should explode
		else if (temp == TileType::BOMB)
		{
			ForceExplosion(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign);
			break;
		}
	}
}

//remove all the explosion colliders in one direction
void RemoveExplosionColliders(int maxDistance,int xPos,int yPos,int xSign,int ySign)
{
	for (int i = 0; i < maxDistance; ++i)
	{
		Collision::RemoveCollisionFromMap(Map::explosionTiles, xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign);
	}
}

//set the timer so the bomb immediatly explodes
void Bomb::SetTimerToExplosion()
{
	startTime = SDL_GetTicks() - mTimeToExplode;
}

//force the bomb found on x,y coordinates to explode
void ForceExplosion(int xPos,int yPos)
{
	list<Bomb*>::iterator it;
	for (it = Player::bombs.begin(); it != Player::bombs.end(); ++it)
	{
		if (!(*it)->explode && (*it)->GetBomb().x == xPos && (*it)->GetBomb().y == yPos)
		{
			(*it)->SetTimerToExplosion();
		}
	}
}