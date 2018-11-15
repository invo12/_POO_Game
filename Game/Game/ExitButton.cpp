#include "ExitButton.h"

ExitButton::ExitButton(int x, int y, int w, int h, const char* textureName,Game *game)
{
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	dest.x = x;
	dest.y = y;
	dest.w = width;
	dest.h = height;
	this->game = game;
	texture = TextureManager::LoadTexture(textureName);
	SetActive(true,true);
}

void ExitButton::OnPressedButton()
{
	game->Stop();
}