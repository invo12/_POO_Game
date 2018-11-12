#include "Menu.h"
#include "StartButton.h"
#include "ExitButton.h"
#include "SwitchPanelsButton.h"

Menu::Menu(Game* game)
{
	panels.clear();
	buttons.clear();
	this->game = game;
}
Menu::~Menu()
{}
void Menu::BuildTheMenu()
{
	Panel* mainPanel = new Panel(0, 0, GameConstants::screenWidth, GameConstants::screenHeight, "Assets/Background.png");;
	StartButton* startButton = new StartButton(0.05 * GameConstants::screenWidth, 0.4 * GameConstants::screenHeight, GameConstants::screenWidth * 0.3, GameConstants::screenHeight * 0.1, "Assets/StartButton.png",game);;
	ExitButton* exitButton = new ExitButton(0.05 * GameConstants::screenWidth, 0.8 * GameConstants::screenHeight, GameConstants::screenWidth * 0.3, GameConstants::screenHeight * 0.1, "Assets/QuitButton.png", game);
	Panel* creditsPanel = new Panel(0, 0, GameConstants::screenWidth, GameConstants::screenHeight, "Assets/CreditsPanel.png");
	SwitchButton* creditsButton = new SwitchButton(0.05 * GameConstants::screenWidth, 0.6 * GameConstants::screenHeight, GameConstants::screenWidth * 0.3, GameConstants::screenHeight * 0.1, "Assets/OptionsButton.png",mainPanel,creditsPanel);
	SwitchButton* creditsBackButton = new SwitchButton(0.35 * GameConstants::screenWidth, 0.8 * GameConstants::screenHeight, GameConstants::screenWidth * 0.3, GameConstants::screenHeight * 0.1, "Assets/BackButton.png", creditsPanel, mainPanel);
	mainPanel->AddChild(startButton);
	mainPanel->AddChild(exitButton);
	mainPanel->AddChild(creditsButton);

	creditsPanel->AddChild(creditsBackButton);

	panels.push_front(mainPanel);
	panels.push_front(creditsPanel);

	buttons.push_front(startButton);
	buttons.push_front(exitButton);
	buttons.push_front(creditsButton);
	buttons.push_front(creditsBackButton);
	mainPanel->SetActive(true, true);
	creditsPanel->SetActive(false, false);
}

void Menu::Update()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	if (event.type == SDL_QUIT)
		game->Stop();
	if (!buttons.empty())
	{
		for (auto it = buttons.begin(); it != buttons.end(); ++it)
		{
			if ((*it)->isActive())
			{
				(*it)->Update(event);
				if ((*it)->ButtonWasPressed())			//:D
					break;
			}
		}
	}
}
void Menu::Render()
{
	//clear the render
	SDL_RenderClear(game->renderer);

	if (!panels.empty())
	{
		for (auto it = panels.begin(); it != panels.end(); ++it)
		{
			if ((*it)->isActive())
			{
				(*it)->Render();
			}
		}
	}
	if (!buttons.empty())
	{
		for (auto it = buttons.begin(); it != buttons.end(); ++it)
		{
			if ((*it)->isActive())
			{
				(*it)->Render();
			}
		}
	}
	//and show all the results to the screen
	SDL_RenderPresent(game->renderer);				//:D
}