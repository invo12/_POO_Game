#pragma once
#include "SDL_image.h"
#include "SDL.h"
class TextureManager
{
	TextureManager() {};
public:
	static SDL_Texture* LoadTexture(const char* fileName);
	static void Draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest);
};