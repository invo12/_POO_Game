#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "MenuElement.h"

enum class MouseButton
{
	MOUSE_OUT = 0,
	MOUSE_OVER = 1,
	MOUSE_DOWN = 2,
	MOUSE_UP = 3,
};

class Button:public MenuElement
{
protected:
	int xPos, yPos;
	int width, height;
	bool hover;
	SDL_Texture* buttonInactive;
	SDL_Texture* buttonActive;
public:
	Button(int x,int y,int w,int h);
	Button();
	//void Render() override;
	void Update(SDL_Event &event);
	virtual void OnPressedButton();
};
