#include "Collision.h"
#include "Map.h"
#include <iostream>

bool Collision::checkCollisions(SDL_Rect a, SDL_Rect b)
{
	//edges
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

//chcek if player is touching something 
bool Collision::touchCollisionTile(SDL_Rect playerCollider, list<Tile*> tiles, list<SDL_Rect> ignoredCollisions)
{
	int i = 0;
	list<Tile*>::iterator itT;
	list<SDL_Rect>::iterator itR;
	bool isIgnored;
	bool interactWithSolidBomb = false;
	for (itT = tiles.begin(); itT != tiles.end(); ++itT)
	{
		isIgnored = false;
		if (checkCollisions(playerCollider, (*itT)->getCollider()))
		{
			for (itR = ignoredCollisions.begin(); itR != ignoredCollisions.end(); ++itR)
			{
				
				if ((*itR).x == (*itT)->getCollider().x && (*itR).y == (*itT)->getCollider().y)
				{
					isIgnored = true;
				}
			}
			if (!isIgnored)
			{
				interactWithSolidBomb = true;
			}
		}
	}
	if ((ignoredCollisions.empty() && !tiles.empty() && interactWithSolidBomb) || interactWithSolidBomb)
		return true;
	return false;
}

bool Collision::touchCollisionTile(SDL_Rect playerCollider, list<Tile*> tiles)
{
	list<Tile*>::iterator it;
	//if player collides with something then stop him
	for (it = tiles.begin(); it != tiles.end(); ++it)
	{
		if (checkCollisions(playerCollider, (*it)->getCollider()))
		{
			return true;
		}
	}
	return false;
}

void Collision::RemoveCollisionFromMap(list<Tile*> &tiles,int xPos,int yPos)
{
	list<Tile*>::iterator it;
	//remove collision from x and y coordinates and put grass there
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

//add collision to a certain list
void Collision::AddCollisionOnMap(list<Tile*> &tiles, int xPos, int yPos,TileType type)
{
	tiles.push_front(new Tile(xPos, yPos, type));
}