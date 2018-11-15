#pragma once
#include "Game.h"
#include "PowerUp.h"
#include <iostream>
#include <list>
using namespace std;

class Map
{
public:

	Map();
	~Map();

	void LoadMap(int mapMatrix[13][17]);
	void DrawMap();

	static list<Tile*> collisionTiles;
	static list<Tile*> explosionTiles;
	static list<Tile*> bombTiles;
	static int totalMapCollisionTiles;
	static TileType GetTileType(int x, int y) 
	{
		return static_cast<TileType>(map[y/GameConstants::tileHeight][x/GameConstants::tileWidth]); 
	}
	static void SetTileType(int x, int y,TileType type)
	{
		map[y / GameConstants::tileHeight][x / GameConstants::tileWidth] = static_cast<int>(type);
	}
	static PowerUpType DestroyBlock(int x,int y);
private:

	SDL_Rect src, dest;
	SDL_Rect bgSrc, bgDest;

	SDL_Texture* mWall;
	SDL_Texture* mBackground;
	SDL_Texture* mDestroyableBlock;

	static int map[13][17];
	void ClearList(list<Tile*> &tiles);
};