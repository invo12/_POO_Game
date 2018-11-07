#include "MenuElement.h"
#include <iostream>
using namespace std;
int MenuElement::elementID = 0;

MenuElement::MenuElement()
{
	src.x = src.y = 0;
	SetActive(false);
}

void MenuElement::Render()
{
	if(active)
		TextureManager::Draw(texture, src, dest);
}

void MenuElement::SetActive(bool active)
{
	this->active = active;
}