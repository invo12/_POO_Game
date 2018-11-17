#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "MenuElement.h"
#include "TextureManager.h"

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
	bool pressed;
	SDL_Texture* buttonInactive;
	SDL_Texture* buttonActive;
public:
	Button(int x,int y,int w,int h, const char* textureName);
	Button();
	~Button() {};
	inline bool ButtonWasPressed()
	{
		return pressed;
	}
	void Update(SDL_Event &event);
	virtual void OnPressedButton();
};
