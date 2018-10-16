#include "Tile.h"
#include "GameConstants.h"

Tile::Tile(int x, int y, TileType tileType)
{
	//set the collider of the tile
	tileCollider.x = x;
	tileCollider.y = y;
	tileCollider.w = GameConstants::tileWidth;
	tileCollider.h = GameConstants::tileHeight;

	//set the type of the tile
	type = tileType;
}

TileType Tile::getType()
{
	return type;
}

SDL_Rect Tile::getCollider()
{
	return tileCollider;
}

