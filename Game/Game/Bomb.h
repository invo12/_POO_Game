#pragma once
#include "TextureManager.h"
#include "GameConstants.h"
#include "Game.h"

class Bomb
{
public:

	static void Init();
	//constructor and deconstructor
	Bomb(int x,int y,int bombPower);
	~Bomb();
	void Update();
	void Render();
	
	//has bomb exploded?
	bool explode = false;
	bool destroy = false;
	bool solid = false;

	//get the position of the bomb
	SDL_Rect GetBomb();
	SDL_Rect GetBombCollider();

	void Explode();
	void MakeBombSolid();
	void SetTimerToExplosion();

	//not solid bombs
	static list<SDL_Rect> notSolidBombs;
private:
	//collider of the bomb
	SDL_Rect mBombCollider;

	//rendering part
	SDL_Rect src, dest;
	static SDL_Texture* mBombTexture;
	static SDL_Texture* mExplosionTexture;
	static SDL_Texture* mExplosionUpTexture;
	static SDL_Texture* mExplosionRightTexture;

	//timer part of the bomb
	Uint32 startTime;
	int mTimeToExplode = 10000;
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
void ForceExplosion(int xPos,int yPos);
void RemoveExplosionColliders(int maxDistance, int xPos, int yPos, int xSign, int ySign);
void AddNotSolidBombOnList(SDL_Rect bomb);
void RemoveSolidBombFromList(SDL_Rect bomb);