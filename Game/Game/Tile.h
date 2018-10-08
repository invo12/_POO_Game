#pragma once
#include "SDL.h"

class Tile
{
public:
	//constructor
	Tile(int x,int y,int tileType);
	
	//method for obtaining the type
	int getType();

	//method for getting the box collider
	SDL_Rect getCollider();

private:
	//collider
	SDL_Rect tileCollider;

	//type of the tile
	int type;
};