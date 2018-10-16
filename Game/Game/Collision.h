#pragma once
#include "Game.h"
#include <list>
using namespace std;

namespace Collision
{
	bool checkCollisions(SDL_Rect a, SDL_Rect b);
	bool touchCollisionTile(SDL_Rect playerCollider, list<Tile*> tiles);
	bool touchCollisionTile(SDL_Rect playerCollider, list<SDL_Rect> tiles, SDL_Rect* currentTile);
	void RemoveCollisionFromMap(list<Tile*> &tiles,int xPos,int yPos);
	void AddCollisionOnMap(list<Tile*> &tiles, int xPos, int yPos, TileType type);
};