#pragma once
#include "Game.h"

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
	PowerUp(PowerUpType powerType, SDL_Rect collider);
	~PowerUp();
	bool Update();
	PowerUpType mType;
private:
	SDL_Rect mCollider;
};