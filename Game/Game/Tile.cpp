#include "Tile.h"
#include "GameConstants.h"

Tile::Tile(int x, int y, TileType tileType)
{
	//set the collider of the tile
	mTileCollider.x = x;
	mTileCollider.y = y;
	mTileCollider.w = GameConstants::tileWidth;
	mTileCollider.h = GameConstants::tileHeight;

	//set the type of the tile
	mType = tileType;
}

TileType Tile::getType()
{
	return mType;
}

SDL_Rect Tile::getCollider()
{
	return mTileCollider;
}

