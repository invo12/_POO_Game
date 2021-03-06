#include "Bomb.h"
#include "Player.h"
#include <iostream>
using namespace std;

SDL_Texture* Bomb::mBombTexture;
SDL_Texture* Bomb::mExplosionTexture;
SDL_Texture* Bomb::mExplosionUpTexture;
SDL_Texture* Bomb::mExplosionRightTexture;

SDL_Texture* Bomb::mExplosionRightEndTexture;
SDL_Texture* Bomb::mExplosionLeftEndTexture;
SDL_Texture* Bomb::mExplosionUpEndTexture;
SDL_Texture* Bomb::mExplosionDownEndTexture;

void Bomb::Init()
{
	mBombTexture = TextureManager::LoadTexture("Assets/bomb.png");
	mExplosionTexture = TextureManager::LoadTexture("Assets/explosion.png");
	mExplosionUpTexture = TextureManager::LoadTexture("Assets/explosion_up.png");
	mExplosionRightTexture = TextureManager::LoadTexture("Assets/explosion_right.png");
	mExplosionRightEndTexture = TextureManager::LoadTexture("Assets/explosion_end_right.png");
	mExplosionLeftEndTexture = TextureManager::LoadTexture("Assets/explosion_end_left.png");
	mExplosionUpEndTexture = TextureManager::LoadTexture("Assets/explosion_end_up.png");
	mExplosionDownEndTexture = TextureManager::LoadTexture("Assets/explosion_end_down.png");
}

Bomb::Bomb(int x,int y,int bombPower,int playerNumber)
{
	//cut the bomb from file
	src.x = src.y = 0;
	src.w = GameConstants::bombSpriteWidth;
	src.h = GameConstants::bombSpriteHeight;

	//place the bomb on the specific tile
	dest.w = GameConstants::tileWidth;
	dest.h = GameConstants::tileHeight;
	dest.x = x;
	dest.y = y;

	//start the bomb timer
	mStartTime = SDL_GetTicks();
	
	//proprietes of the bomb
	mBombPower = bombPower;

	//set the position of the bomb
	xPos = x;
	yPos = y;

	mParentPlayer = playerNumber;
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
	Game::totalNumberOfBombs--;
}

bool Bomb::Update()
{
	mCurrentTime = SDL_GetTicks() - mStartTime;
	//if time has run out then explode
	if (!explode && SDL_GetTicks() - mStartTime > mTimeToExplode)
	{
		explode = true;
		Explode();
	}
	//after it explodes t\wait some time before cleaning the explosion
	else if(explode && SDL_GetTicks() - mStartTime > mTimeToExplode + mTimeToDestroyAfterExplosion)
	{
		destroy = true;
	}
	return !destroy;
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
			if (i == mMaxUp - 1)
			{
				TextureManager::Draw(mExplosionUpEndTexture, src, dest);
				break;
			}
			TextureManager::Draw(mExplosionUpTexture, src, dest);
		}

		//down
		setDestination(dest, xPos, yPos);
		for (int i = 0; i < mMaxDown; ++i)
		{
			dest.y += GameConstants::tileHeight;
			if (i == mMaxDown - 1)
			{
				TextureManager::Draw(mExplosionDownEndTexture, src, dest);
				break;
			}
			TextureManager::Draw(mExplosionUpTexture, src, dest);
		}

		//right
		setDestination(dest, xPos, yPos);
		for (int i = 0; i < mMaxRight; ++i)
		{
			dest.x += GameConstants::tileWidth;
			if (i == mMaxRight - 1)
			{
				TextureManager::Draw(mExplosionRightEndTexture, src, dest);
				break;
			}
			TextureManager::Draw(mExplosionRightTexture, src, dest);
		}

		//left
		setDestination(dest, xPos, yPos);
		for (int i = 0; i < mMaxLeft; ++i)
		{
			dest.x -= GameConstants::tileWidth;
			if (i == mMaxLeft - 1)
			{
				TextureManager::Draw(mExplosionLeftEndTexture, src, dest);
				break;
			}
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
	bool wall = false;
	list<PowerUp*>::iterator it;
	for (int i = 0; i < bombPower; ++i)
	{
		temp = Map::GetTileType(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i+1) * GameConstants::tileHeight * ySign);
		//if it's grass ignore it and continue
		switch (temp)
		{
		case TileType::GRASS:
			maxDistance++;
			Map::SetTileType(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign, TileType::EXPLOSION);
			Collision::AddCollisionOnMap(Map::explosionTiles, xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign, TileType::EXPLOSION);
			break;
			//if it's wall then you can't pass
		case TileType::WALL:
			wall = true;
			break;

		//if it's other bomb it should explode
		case TileType::BOMB:
			ForceExplosion(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign);
			wall = true;
			break;

		case TileType::EXPLOSION:
			wall = true;
			break;
		case TileType::POWERUP:
			maxDistance++;
			wall = true;
			for (it = Game::powerUps.begin(); it != Game::powerUps.end(); ++it)
			{
				if ((*it)->GetPosition().x == xPos + (i + 1) * GameConstants::tileWidth * xSign && (*it)->GetPosition().y == yPos + (i + 1) * GameConstants::tileHeight * ySign)
				{
					delete (*it);
					(*it) = nullptr;
					Game::powerUps.erase(it);
					Map::SetTileType(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign, TileType::GRASS);
					break;
				}
			}
			break;
			//if it's a destroyable block then make the explosion hover it then stop 
		case TileType::DESTROYABLEBLOCK:
			Map::SetTileType(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign, TileType::EXPLOSION);
			maxDistance++;
			PowerUpType powerUpType = Map::DestroyBlock(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign);
			if (powerUpType != PowerUpType::NOTHING)
			{
				SDL_Rect collider;
				collider.x = xPos + (i + 1) * GameConstants::tileWidth * xSign;
				collider.y = yPos + (i + 1) * GameConstants::tileHeight * ySign;
				collider.w = GameConstants::tileWidth;
				collider.h = GameConstants::tileHeight;
				Game::powerUps.push_front(new PowerUp(powerUpType, collider));
				Map::SetTileType(xPos + (i + 1) * GameConstants::tileWidth * xSign, yPos + (i + 1) * GameConstants::tileHeight * ySign, TileType::POWERUP);
			}
			wall = true;
			break;
		}
		if (wall)
			break;
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
	mStartTime = SDL_GetTicks() - mTimeToExplode;
}

//force the bomb found on x,y coordinates to explode
void ForceExplosion(int xPos,int yPos)
{
	list<Bomb*>::iterator it;
	for (it = Game::bombs.begin(); it != Game::bombs.end(); ++it)
	{
		if (!(*it)->HasBombExploded() && (*it)->GetBomb().x == xPos && (*it)->GetBomb().y == yPos)
		{
			(*it)->SetTimerToExplosion();
		}
	}
}
bool Bomb::HasBombExploded()
{
	return explode;
}

int Bomb::GetParent()
{
	return mParentPlayer;
}

void Bomb::ResumeTimer()
{
	mStartTime = SDL_GetTicks() - mCurrentTime;
}