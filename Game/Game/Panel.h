#pragma once
#include "MenuElement.h"

class Panel :public MenuElement
{
public:
	Panel(int x,int y,int w,int h, const char* textureName);
	~Panel() {};
};