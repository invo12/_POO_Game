#pragma once
#include "SDL.h"
#include "GameConstants.h"
enum class TileType
{
	GRASS = 0,
	WALL = 1,
	DESTROYABLEBLOCK = 2,
	BOMB = 3,
	EXPLOSION = 4,
	POWERUP = 5
};

class Tile
{
public:
	//constructor
	Tile(int x,int y,TileType tileType);
	~Tile() {};
	//method for obtaining the type
	TileType getType();

	//method for getting the box collider
	SDL_Rect getCollider();

private:
	//collider
	SDL_Rect mTileCollider;

	//type of the tile
	TileType mType;
};
