#pragma once
#include "Button.h"
#include "Game.h"

class ExitButton :public Button
{
public:
	ExitButton(int x, int y, int w, int h, const char* textureName,Game* game);
	~ExitButton() {};
private:
	void OnPressedButton() override;
	Game* game;
};