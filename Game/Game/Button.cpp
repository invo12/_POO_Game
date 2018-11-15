#include "Button.h"
#include "TextureManager.h"
#include <iostream>
using namespace std;

Button::Button(int x,int y,int w,int h,const char* textureName)
{
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	dest.x = x;
	dest.y = y;
	dest.w = width;
	dest.h = height;
	buttonActive = TextureManager::LoadTexture("Assets/wall.png");
	buttonInactive = TextureManager::LoadTexture("Assets/grass.png");
	texture = TextureManager::LoadTexture(textureName);
}
Button::Button()
{
	src.w = 256;
	src.h = 64;
}
//void Button::Render()
//{
//	if (hover)
//		TextureManager::Draw(buttonActive, src, dest);
//	else
//		TextureManager::Draw(buttonInactive, src, dest);
//}

void Button::Update(SDL_Event &event)
{
	pressed = false;
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
	active = !active;
}