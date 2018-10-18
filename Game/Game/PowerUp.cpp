#include "PowerUp.h"
#include "Player.h"
#include "Collision.h"

PowerUp::PowerUp(PowerUpType powerType,SDL_Rect collider)
{
	mType = powerType;
	mCollider = collider;
}
PowerUp::~PowerUp()
{
	cout << "collected";
}

bool PowerUp::Update()
{
	for (int i = 0; i < Player::numberOfPlayers; ++i)
	{
		//did the player touched the powerUp?
		if (Collision::checkCollisions(Player::players[i]->GetCollider(), mCollider))
		{
			return true;
		}
	}
	return false;
}