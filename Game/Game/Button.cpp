#include "Button.h"
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
	src.w = GameConstants::menuButtonWidth;
	src.h = GameConstants::menuButtonHeight;
}

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