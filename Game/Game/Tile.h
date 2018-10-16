#pragma once
#include "SDL.h"


enum class TileType
{
	GRASS = 0,
	WALL = 1,
	DESTROYABLEBLOCK = 2,
	BOMB = 3,
	EXPLOSION = 4
};

class Tile
{
public:
	//constructor
	Tile(int x,int y,TileType tileType);
	
	//method for obtaining the type
	TileType getType();

	//method for getting the box collider
	SDL_Rect getCollider();

private:
	//collider
	SDL_Rect tileCollider;

	//type of the tile
	TileType type;
};
