#include "ExitButton.h"

ExitButton::ExitButton(int x, int y, int w, int h,Game *game)
{
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	src.x = src.y = 0;
	src.w = src.h = 64;
	dest.x = x;
	dest.y = y;
	dest.w = width;
	dest.h = height;
	this->game = game;
	texture = TextureManager::LoadTexture("Assets/wall.png");
}

void ExitButton::OnPressedButton()
{
	SetActive(true);
	game->Stop();
}