#pragma once

#include "Graphics.h"
#include "AnimatedSprite.h"
#include "TextManager.h"
#include "SDL.h"
#include "sprite.h"
#include "Input.h"

class Graphics;

class Title : public AnimatedSprite, public TextManager
{
public:
	Title();
	Title(Graphics &graphics, Input &input, SDL_Event &event);
	~Title();
	void playNext(int num);
	bool Start(Graphics &graphics, Input &input, SDL_Event &event);
	void animationDone(std::string currentAnimation);
	void setupAnimations();
	void update(float elapsedTime);
	void draw(Graphics & graphics);
	int getMenuChoice();
private:
	Graphics _graphics; //store graphics object
	Sprite _title;
	Sprite _startGame;
	Sprite _loadGame;
	Sprite _settings;
	Sprite _settingsMenu;
	Sprite _selectionBox;

	int selectX = 185;
	int selectY = 275;
	int menuChoice = 0;
	int settingsChoice = 0;
	int volumePercent = 100;

	bool showSettings = false;
	bool changeVolume = false;
	bool isSubmenu = false;
};

