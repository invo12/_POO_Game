#include "SwitchPanelsButton.h"
#include<iostream>
using namespace std;
SwitchButton::SwitchButton(int x, int y, int w, int h, const char* textureName,Panel* current,Panel* next)
{
	xPos = x;
	yPos = y;
	width = w;
	height = h;
	dest.x = x;
	dest.y = y;
	dest.w = width;
	dest.h = height;
	texture = TextureManager::LoadTexture(textureName);
	currentPanel = current;
	nextPanel = next;
	SetActive(true, true);
}

void SwitchButton::OnPressedButton()
{
	currentPanel->SetActive(false,false);
	nextPanel->SetActive(true, true);
	pressed = true;
}