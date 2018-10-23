#include "TextureManager.h"
#include "Game.h"
SDL_Texture* TextureManager::LoadTexture(const char* fileName)
{
	SDL_Surface* tempSurface = IMG_Load(fileName);										//memory zone that we can draw onto(manipulate it's pixels
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);	//creating the texture on that surface
	SDL_FreeSurface(tempSurface);														//free the memory occupied by our surface

	return texture;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest)
{
	SDL_RenderCopy(Game::renderer, texture, &src, &dest);	//render the texture
}