#pragma once
#include "Button.h"
#include "Game.h"

class StartButton :public Button
{
public:
	StartButton(int x, int y, int w, int h, const char* textureName, Game* game);
private:
	void OnPressedButton() override;
	Game* game;
};