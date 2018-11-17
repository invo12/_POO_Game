#include "MenuElement.h"
using namespace std;
int MenuElement::elementID = 0;

MenuElement::MenuElement()
{
	src.x = src.y = 0;
	src.w = GameConstants::tileWidth;
	src.h = GameConstants::tileHeight;
	children.clear();
	parent = nullptr;
	SetActive(false,false);
}

void MenuElement::Render()
{
	if (active)
	{
		TextureManager::Draw(texture, src, dest);
	}
}

void MenuElement::SetActive(bool active,bool activeState)
{
	this->active = active;
	this->activeState = activeState;
	if (!children.empty())
	{
		for (MenuElement* child : children)
		{
			child->SetActive(active ? (child->activeState) : false,child->activeState);
		}
	}
}

void MenuElement::AddChild(MenuElement* child)
{
	if (child != nullptr)
	{
		children.push_back(child);
		child->MakeParent(this);
	}
}

void MenuElement::MakeParent(MenuElement* parent)
{
	if(parent != nullptr)
		this->parent = parent;
}