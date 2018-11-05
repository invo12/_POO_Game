#include "Button.h"
#include "TextureManager.h"
#include <iostream>
using namespace std;

Button::Button(int x,int y,int w,int h)
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
	buttonActive = TextureManager::LoadTexture("Assets/wall.png");
	buttonInactive = TextureManager::LoadTexture("Assets/grass.png");
}

void Button::Render()
{
	if (hover)
		TextureManager::Draw(buttonActive, src, dest);
	else
		TextureManager::Draw(buttonInactive, src, dest);
}

void Button::Update(SDL_Event &event)
{
	if (event.type == SDL_MOUSEBUTTONUP)
	{
		int x = event.button.x;
		int y = event.button.y;

		if ((x > xPos) && (x < xPos + width) && (y > yPos) && (y < yPos + height))
		{
			OnPressedButton();
		}
	}
}

void Button::OnPressedButton()
{
	hover = !hover;	
}