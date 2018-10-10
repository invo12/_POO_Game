#include "Map.h"
#include "TextureManager.h"
#include <iostream>
using namespace std;

int Map::map[13][17];

int lvl1[13][17] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,2,2,2,2,2,2,2,2,2,2,2,0,0,1},
	{1,0,1,0,1,2,1,0,1,2,1,0,1,0,1,0,1},
	{1,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,1},
	{1,2,1,0,1,0,1,2,1,0,1,2,1,0,1,2,1},
	{1,2,2,0,2,0,0,0,0,0,0,0,2,2,2,2,1},
	{1,2,1,0,1,0,1,0,1,2,1,0,1,2,1,0,1},
	{1,2,2,0,0,0,2,2,2,2,2,0,2,2,2,2,1},
	{1,0,1,2,1,0,1,2,1,0,1,0,1,0,1,0,1},
	{1,2,2,2,2,2,2,2,2,2,2,0,2,2,2,2,1},
	{1,0,1,0,1,2,1,0,1,2,1,0,1,2,1,0,1},
	{1,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

Map::Map()
{
	brick = TextureManager::LoadTexture("Assets/brick.png");
	grass = TextureManager::LoadTexture("Assets/grass.png");
	wall = TextureManager::LoadTexture("Assets/wall.png");

	LoadMap(lvl1);
	src.w = dest.w = 64;
	src.h = dest.h = 64;

	src.x = src.y = 0;
	dest.x = dest.y = 0;
}

int Map::totalMapCollisionTiles = 0;
Tile* Map::collisionTiles[165] = { nullptr };

void Map::LoadMap(int mapMatrix[13][17])
{
	int k = 0;
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			map[i][j] = mapMatrix[i][j];								//copy the map
			int type = map[i][j];										//get type of the tile
			if (type == 0 || i == 0 || j == 0 || i == 12 || j == 16)	//if it's not grass or in the outer rectangle
				continue;
			collisionTiles[k++] = new Tile(j*64, i*64, type);			//add to the collision vector
		}
	}
	totalMapCollisionTiles = k;									//save the total number of tiles
}

void Map::DrawMap()
{
	int type = 0;
	int k = 0;

	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			type = map[i][j];
			dest.x = j * 64;
			dest.y = i * 64;
			switch (type)
			{
			case 0:
				TextureManager::Draw(grass, src, dest);
				break;
			case 1:
				TextureManager::Draw(brick, src, dest);
				break;
			case 2:
				TextureManager::Draw(wall, src, dest);
				break;
			}

		}
	}
}