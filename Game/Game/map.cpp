#include "Map.h"
#include "TextureManager.h"

int lvl1[13][17] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
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

void Map::LoadMap(int mapMatrix[13][17])
{
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			map[i][j] = mapMatrix[i][j];
		}
	}
}

void Map::DrawMap()
{
	int type = 0;

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