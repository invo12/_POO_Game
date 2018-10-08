#pragma once
#include "Game.h"

namespace Collision
{
	bool checkCollisions(SDL_Rect a, SDL_Rect b);
	bool touchCollisionTile(SDL_Rect playerCollider, Tile* tiles[]);
};