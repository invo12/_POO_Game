#pragma once
#include "SDL.h"
#include "SDL_image.h"

enum class MouseButton
{
	MOUSE_OUT = 0,
	MOUSE_OVER = 1,
	MOUSE_DOWN = 2,
	MOUSE_UP = 3,
};

class Button
{
private:
	int xPos, yPos;
	int width, height;
	bool hover;
	SDL_Texture* buttonInactive;
	SDL_Texture* buttonActive;
	SDL_Rect src, dest;
public:
	Button(int x,int y,int w,int h);
	void Render();
	void Update(SDL_Event &event);
	void OnPressedButton();
};
