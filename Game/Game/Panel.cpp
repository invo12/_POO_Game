#include "Panel.h"
using namespace std;
Panel::Panel(int x,int y,int w,int h,const char* textureName)
{
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	src.w = width;
	src.h = height;
	dest.x = x;
	dest.y = y;
	dest.w = width;
	dest.h = height;
	texture = TextureManager::LoadTexture(textureName);
	SetActive(true,true);
}
