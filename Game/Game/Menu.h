#pragma once
#include "MenuElement.h"
#include "Game.h"
#include "Button.h"
#include "Panel.h"
#include "StartButton.h"
#include "ExitButton.h"
#include "SwitchPanelsButton.h"
#include <list>
using namespace std;

class Menu
{
public:
	Menu(Game* game);
	~Menu();
	void BuildTheMenu();
	void Update();
	void Render();
private:
	list<Panel*> panels;
	list<Button*> buttons;
	Game* game;
};