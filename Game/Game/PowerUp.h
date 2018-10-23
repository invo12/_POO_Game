#pragma once
#include "SDL_image.h"

enum class PowerUpType
{
	BOMB,
	FIRE,
	SPEED,
	NOTHING
};

class PowerUp
{
public:
	static void Init();
	PowerUp(PowerUpType powerType, SDL_Rect collider);
	~PowerUp();
	int Update();
	PowerUpType mType;
	void Render();

private:
	SDL_Rect mCollider;

	//rendering
	SDL_Rect mSrc, mDest;

	static SDL_Texture* speedTexture;
	static SDL_Texture* bombTexture;
	static SDL_Texture* fireTexture;
};