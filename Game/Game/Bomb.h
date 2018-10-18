#pragma once
#include "TextureManager.h"
#include "GameConstants.h"
#include "Game.h"

class Bomb
{
public:

	//load textures function
	static void Init();
	//constructor and deconstructor
	Bomb(int x,int y,int bombPower);
	~Bomb();

	//basic functions
	void Update();
	void Render();
	
	//has bomb exploded?
	bool explode = false;
	bool destroy = false;

	//get the position of the bomb
	SDL_Rect GetBomb();

	//methods that make the bombs to explode imediatly
	void Explode();
	void SetTimerToExplosion();

	bool ignored = false;
	int set[3] = { 0 };
private:

	//rendering part
	SDL_Rect src, dest;
	static SDL_Texture* mBombTexture;
	static SDL_Texture* mExplosionTexture;
	static SDL_Texture* mExplosionUpTexture;
	static SDL_Texture* mExplosionRightTexture;

	//timer part of the bomb
	Uint32 startTime;
	int mTimeToExplode = 2000;
	int mTimeToDestroyAfterExplosion = 600;

	//proprietes
	int mBombPower;
	int mMaxLeft = 0;
	int mMaxRight = 0;
	int mMaxUp = 0;
	int mMaxDown = 0;

	//positions
	int xPos, yPos;

};

//reset dest rectangle
void setDestination(SDL_Rect &dest, int x, int y);

//get max distance in one direction that the bomb can explode
void GetMaxDistanceInDirection(int &maxDistance, int bombPower, int xPos, int yPos, int xSign, int ySign);

//force the bomb from x,y coordinates to explode
void ForceExplosion(int xPos,int yPos);

//remove all the explosion colliders created by the bomb
void RemoveExplosionColliders(int maxDistance, int xPos, int yPos, int xSign, int ySign);