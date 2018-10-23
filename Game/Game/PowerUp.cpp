#include "PowerUp.h"
#include "Player.h"
#include "Collision.h"
#include "PowerUp.h"

SDL_Texture* PowerUp::speedTexture;
SDL_Texture* PowerUp::bombTexture;
SDL_Texture* PowerUp::fireTexture;

//initialize textures for the power ups
void PowerUp::Init()
{
	speedTexture = TextureManager::LoadTexture("Assets/speedUp.png");
	bombTexture = TextureManager::LoadTexture("Assets/bombUp.png");
	fireTexture = TextureManager::LoadTexture("Assets/fireUp.png");
}

PowerUp::PowerUp(PowerUpType powerType,SDL_Rect collider)
{
	//set the type of power up
	mType = powerType;

	//his bounds
	mCollider = collider;

	//and his rendering rectangles
	mSrc.x = mSrc.y = 0;
	mDest.x = collider.x;
	mDest.y = collider.y;
	mSrc.h = mDest.h = GameConstants::tileHeight;
	mSrc.w = mDest.w = GameConstants::tileWidth;
}
PowerUp::~PowerUp()
{
	cout << "collected";
}

int PowerUp::Update()
{
	for (int i = 0; i < 2; ++i)
	{
		//did the player touched the powerUp?
		if (Collision::checkCollisions(Player::players[i]->GetCollider(), mCollider))
		{
			return i;
		}
	}
	return -1;
}

//render the power up
void PowerUp::Render()
{
	if (mType == PowerUpType::BOMB)
	{
		TextureManager::Draw(bombTexture, mSrc, mDest);
	}
	else if (mType == PowerUpType::FIRE)
	{
		TextureManager::Draw(fireTexture, mSrc, mDest);
	}
	else if (mType == PowerUpType::SPEED)
	{
		TextureManager::Draw(speedTexture, mSrc, mDest);
	}
}