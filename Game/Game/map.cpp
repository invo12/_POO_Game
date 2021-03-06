#include "Map.h"

using namespace std;

int Map::map[13][17];
int Map::totalMapCollisionTiles = 0;
list<Tile*> Map::collisionTiles{};
list<Tile*> Map::explosionTiles{};
list<Tile*> Map::bombTiles{};

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
	mWall = TextureManager::LoadTexture("Assets/wall.png");
	mBackground = TextureManager::LoadTexture("Assets/mapBackground.png");
	mDestroyableBlock = TextureManager::LoadTexture("Assets/destroyable_block.png");

	LoadMap(lvl1);
	bgSrc.w = bgDest.w = GameConstants::screenWidth;
	bgSrc.h = bgDest.h = GameConstants::screenHeight;
	src.w = dest.w = GameConstants::tileWidth;
	src.h = dest.h = GameConstants::tileHeight;

	src.x = src.y = bgSrc.x = bgSrc.y = 0;
	dest.x = dest.y = bgDest.x = bgDest.y = 0;
}
Map::~Map()
{
	ClearList(collisionTiles);
	ClearList(explosionTiles);
	ClearList(bombTiles);
}

void Map::LoadMap(int mapMatrix[13][17])
{
	collisionTiles.clear();
	int k = 0;
	list<Tile*>::iterator it = collisionTiles.begin();
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			map[i][j] = mapMatrix[i][j];								//copy the map
			int type = map[i][j];										//get type of the tile
			if (type == static_cast<int>(TileType::GRASS) || i == 0 || j == 0 || i == 12 || j == 16)	//if it's not grass or in the outer rectangle
				continue;
			collisionTiles.push_front(new Tile(j * GameConstants::tileWidth, i * GameConstants::tileHeight, static_cast<TileType>(type)));
		}
	}
	totalMapCollisionTiles = k;									//save the total number of tiles
}

void Map::DrawMap()
{
	TileType type;
	int k = 0;

	TextureManager::Draw(mBackground, bgSrc, bgDest);

	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			type = static_cast<TileType>(map[i][j]);
			dest.x = j * GameConstants::tileWidth;
			dest.y = i * GameConstants::tileHeight;
			switch (type)
			{
			case TileType::DESTROYABLEBLOCK:
				TextureManager::Draw(mDestroyableBlock, src, dest);
				break;
			case TileType::WALL:
				TextureManager::Draw(mWall, src, dest);
				break;
			}
		}
	}
}

PowerUpType Map::DestroyBlock(int x, int y)
{
	map[y / GameConstants::tileHeight][x / GameConstants::tileWidth] = static_cast<int>(TileType::GRASS);
	Collision::RemoveCollisionFromMap(collisionTiles, x, y);
	int r = rand() % 100;
	if (r < GameConstants::chanceForExtraBomb)
	{
		return PowerUpType::BOMB;
	}
	else if (r < GameConstants::chanceForExtraFire)
	{
		return PowerUpType::FIRE;
	}
	else if (r < GameConstants::chanceForExtraSpeed)
	{
		return PowerUpType::SPEED;
	}
	else
	{
		return PowerUpType::NOTHING;
	}
}

void Map::ClearList(list<Tile*> &tiles)
{
	if (!tiles.empty())
	{
		list<Tile*>::iterator it;
		it = tiles.begin();
		while (it != tiles.end())
		{
			if (*it != nullptr)
			{
				delete(*it);
				*it = nullptr;
			}
			tiles.pop_front();
			it = tiles.begin();
		}
		cout << "\ntiles Cleared";

	}
}