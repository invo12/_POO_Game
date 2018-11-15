#include "StartButton.h"

StartButton::StartButton(int x, int y, int w, int h, const char* textureName,Game* game)
{
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	dest.x = x;
	dest.y = y;
	dest.w = width;
	dest.h = height;
	texture = TextureManager::LoadTexture(textureName);
	SetActive(true,true);
	this->game = game;
}

void StartButton::OnPressedButton()
{
	if (parent != nullptr && game != nullptr)
	{
		game->Resume();
	}
}