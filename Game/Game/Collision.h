#pragma once
#include "Game.h"
#include <list>
using namespace std;

namespace Collision
{
	//check if 2 rectangles are colliding
	bool checkCollisions(SDL_Rect a, SDL_Rect b);

	//check if the player is touching any collision tile/bomb/explosion
	bool touchCollisionTile(SDL_Rect playerCollider, list<Tile*> tiles, list<SDL_Rect> ignoredCollisions);
	bool touchCollisionTile(SDL_Rect playerCollider, list<Tile*> tiles);

	//remove collision from certain list placed at x,y position
	void RemoveCollisionFromMap(list<Tile*> &tiles,int xPos,int yPos);

	//add collision on certain list placed at x,y position
	void AddCollisionOnMap(list<Tile*> &tiles, int xPos, int yPos, TileType type);
};