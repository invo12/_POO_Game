#pragma once
#include "TextureManager.h"
#include "SDL_image.h"

class MenuElement
{
public:
	MenuElement();
	SDL_Texture* texture;
	virtual void Render();
	void SetActive(bool active);
protected:
	static int elementID;
	SDL_Rect src, dest;
	bool active;
};