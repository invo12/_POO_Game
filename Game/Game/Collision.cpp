#include "Collision.h"
#include "Map.h"
#include <iostream>

bool Collision::checkCollisions(SDL_Rect a, SDL_Rect b)
{
	int aLeft, bLeft;
	int aRight, bRight;
	int aTop, bTop;
	int aBot, bBot;

	//calculate a edges
	aLeft = a.x;
	aTop = a.y;
	aRight = a.x + a.w;
	aBot = a.y + a.h;

	//calculate b edges
	bLeft = b.x;
	bTop = b.y;
	bRight = b.x + b.w;
	bBot = b.y + b.h;

	//consider each case
	//on the first two if's the sign is counter intuitive because the y axis is flipped
	if (aBot <= bTop)
	{
		return false;
	}
	if (aTop >= bBot)
	{
		return false;
	}
	if (aLeft >= bRight)
	{
		return false;
	}
	if (aRight <= bLeft)
	{
		return false;
	}
	//if there is and intersection then objects collide
	return true;
}

bool Collision::touchCollisionTile(SDL_Rect playerCollider, list<Tile*> tiles)
{
	list<Tile*>::iterator it;
	for (it = tiles.begin(); it != tiles.end(); ++it)
	{
		if (checkCollisions(playerCollider, (*it)->getCollider()))
		{
			return true;
		}
	}

	return false;
}

bool Collision::touchCollisionTile(SDL_Rect playerCollider, list<SDL_Rect> bombs, SDL_Rect* currentBomb)
{
	list<SDL_Rect>::iterator it;
	if (currentBomb != nullptr)
	{
		for (it = bombs.begin(); it != bombs.end(); ++it)
		{
			if (!SDL_RectEquals(currentBomb, &(*it)) && checkCollisions(playerCollider, (*it)))
			{
				return true;
			}
		}
	}
	else
	{
		for (it = bombs.begin(); it != bombs.end(); ++it)
		{
			if (abs(playerCollider.x - (*it).x) < 60 && (abs(playerCollider.y - (*it).y) < 60))
				return false;
			if ( checkCollisions(playerCollider, (*it)))
			{
				return true;
			}
		}
	}
	return false;
}

void Collision::RemoveCollisionFromMap(list<Tile*> &tiles,int xPos,int yPos)
{
	list<Tile*>::iterator it;
	for (it = tiles.begin(); it != tiles.end(); ++it)
	{
		if ((*it)->getCollider().x == xPos && (*it)->getCollider().y == yPos)
		{
			Map::SetTileType(xPos, yPos,TileType::GRASS);
			Tile* temp = *it;
			tiles.remove(*it);
			delete temp;
			return;
		}
	}
}

void Collision::AddCollisionOnMap(list<Tile*> &tiles, int xPos, int yPos,TileType type)
{
	tiles.push_front(new Tile(xPos, yPos, type));
}