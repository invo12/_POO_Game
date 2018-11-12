#pragma once
#include "Button.h"
#include "Panel.h"

class SwitchButton:public Button
{
public:
	SwitchButton(int x, int y, int w, int h, const char* textureName, Panel* current, Panel* next);
private:
	void OnPressedButton() override;
	Panel* currentPanel;
	Panel* nextPanel;
};