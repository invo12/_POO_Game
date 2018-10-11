#pragma once
#include "TextureManager.h"
#include "GameConstants.h"
#include "Game.h"

class Bomb
{
public:
	//constructor and deconstructor
	Bomb(int x,int y,int bombPower);
	~Bomb();
	void Update();
	void Render();
	
	//has bomb exploded?
	bool explode = false;
	bool destroy = false;
	
	//get the position of the bomb
	SDL_Rect GetBomb();

	void Explode();
private:
	//collider of the bomb
	SDL_Rect bombCollider;

	//rendering part
	SDL_Rect src, dest;
	SDL_Texture* mBombTexture;
	SDL_Texture* mExplosionTexture;
	SDL_Texture* mExplosionUpTexture;
	SDL_Texture* mExplosionRightTexture;

	//timer part of the bomb
	Uint32 startTime;
	int mTimeToExplode = 2000;
	int mTimeToDestroyAfterExplosion = 500;

	//proprietes
	int mBombPower;
	int mMaxLeft = 0;
	int mMaxRight = 0;
	int mMaxUp = 0;
	int mMaxDown = 0;

	//positions
	int xPos, yPos;
};

void setDestination(SDL_Rect &dest, int x, int y);
void GetMaxDistanceInDirection(int &maxDistance, int bombPower, int xPos, int yPos, int xSign, int ySign);