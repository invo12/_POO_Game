#include "Bomb.h"
#include "Map.h"
#include "Player.h"
#include <iostream>
using namespace std;
SDL_Texture* Bomb::mBombTexture;
SDL_Texture* Bomb::mExplosionTexture;
SDL_Texture* Bomb::mExplosionUpTexture;
SDL_Texture* Bomb::mExplosionRightTexture;
list<SDL_Rect> Bomb::notSolidBombs{};

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

	xPos = x;
	yPos = y;
}

Bomb::~Bomb()
{
	if (!solid)
	{
		Map::SetTileType(xPos, yPos, TileType::GRASS);
		RemoveSolidBombFromList(this->GetBomb());
	}
	//up
	RemoveExplosionColliders(mMaxUp, xPos, yPos, 0, -1);

	//down
	RemoveExplosionColliders(mMaxDown, xPos, yPos, 0, 1);

	//left
	RemoveExplosionColliders(mMaxLeft, xPos, yPos, -1, 0);

	//right
	RemoveExplosionColliders(mMaxRight, xPos, yPos, 1, 0);

	Map::DestroyBlock(xPos, yPos);
	Collision::RemoveCollisionFromMap(Map::explosionTiles, xPos, yPos);
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

	Map::SetTileType(xPos, yPos, TileType::EXPLOSION);
	Collision::AddCollisionOnMap(Map::explosionTiles, xPos, yPos, TileType::EXPLOSION);
}

void setDestination(SDL_Rect &dest,int x,int y)
{
	dest.x = x;
	dest.y = y;
}

void GetMaxDistanceInDirection(int &maxDistance,int bombPower,int xPos,int yPos,int xSign,int ySign)
{
	TileType temp;
	for (int i = 0; i < bombPower; ++i)
	{
		temp = Map::GetTileType(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i+1) * GameConstants::tileHeight * ySign);
		if (temp == TileType::GRASS)
		{
			maxDistance++;
			Map::SetTileType(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign, TileType::EXPLOSION);
			Collision::AddCollisionOnMap(Map::explosionTiles, xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign, TileType::EXPLOSION);
		}
		else if (temp == TileType::WALL)
		{
			break;
		}
		else if (temp == TileType::DESTROYABLEBLOCK)
		{
			Map::SetTileType(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign, TileType::EXPLOSION);
			maxDistance++;
			Map::DestroyBlock(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign);
			break;
		}
		else if (temp == TileType::BOMB)
		{
			ForceExplosion(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign);
			break;
		}
	}
}

void RemoveExplosionColliders(int maxDistance,int xPos,int yPos,int xSign,int ySign)
{
	for (int i = 0; i < maxDistance; ++i)
	{
		Collision::RemoveCollisionFromMap(Map::explosionTiles, xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign);
	}
}

void Bomb::MakeBombSolid()
{
	mBombCollider.x = xPos;
	mBombCollider.y = yPos;
	mBombCollider.w = GameConstants::tileWidth;
	mBombCollider.h = GameConstants::tileHeight;
	solid = true;
}

SDL_Rect Bomb::GetBombCollider()
{
	return mBombCollider;
}

void Bomb::SetTimerToExplosion()
{
	startTime = SDL_GetTicks() - mTimeToExplode;
}

void ForceExplosion(int xPos,int yPos)
{
	for (int i = 0; i < Player::totalNumberOfBombs; ++i)
	{
		if (Player::bomb[i] != nullptr)
		{
			if (!Player::bomb[i]->explode && Player::bomb[i]->GetBomb().x == xPos && Player::bomb[i]->GetBomb().y == yPos)
			{
				Player::bomb[i]->SetTimerToExplosion();
			}
		}
	}
}

void AddNotSolidBombOnList(SDL_Rect bomb)
{
	Bomb::notSolidBombs.push_front(bomb);
}

void RemoveSolidBombFromList(SDL_Rect bomb)
{
	list<SDL_Rect>::iterator it;
	for (it = Bomb::notSolidBombs.begin(); it != Bomb::notSolidBombs.end(); ++it)
	{
		if(SDL_RectEquals(&bomb,&(*it)))
			Bomb::notSolidBombs.erase(it);
		break;
	}
}