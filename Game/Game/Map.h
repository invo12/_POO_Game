#pragma once
#include "Game.h"
class Map
{
public:

	Map();
	~Map();

	void LoadMap(int mapMatrix[13][17]);
	void DrawMap();
	static Tile* collisionTiles[165];
	static int totalMapCollisionTiles;
private:

	SDL_Rect src, dest;

	SDL_Texture* brick;
	SDL_Texture* grass;
	SDL_Texture* wall;

	int map[20][25];
};