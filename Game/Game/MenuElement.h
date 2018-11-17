#pragma once
#include "TextureManager.h"
#include "SDL_image.h"
#include "GameConstants.h"
#include <list>

using namespace std;

class MenuElement
{
public:
	MenuElement();
	~MenuElement() {};
	SDL_Texture* texture;
	virtual void Render();
	void SetActive(bool active,bool activeState);
	void AddChild(MenuElement* child);
	void MakeParent(MenuElement* parent);
	inline bool isActive()
	{
		return active;
	}
protected:
	static int elementID;
	SDL_Rect src, dest;
	bool active;
	bool activeState;
	int xPos, yPos;
	int width, height;
	list<MenuElement*> children;
	MenuElement* parent;
};