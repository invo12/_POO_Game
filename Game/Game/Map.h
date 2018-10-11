#pragma once
#include "Game.h"
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
	static int totalMapCollisionTiles;
	static int GetTileType(int x, int y) 
	{
		//std::cout << x / 64 << ' ' << y / 64<<' '<<map[x/64][y/64] << std::endl;
		return map[y/64][x/64]; 
	}
	static void DestroyBlock(int x,int y);
private:

	SDL_Rect src, dest;

	SDL_Texture* brick;
	SDL_Texture* grass;
	SDL_Texture* wall;

	static int map[13][17];
};