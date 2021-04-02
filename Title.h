#pragma once

#include "Graphics.h"
#include "AnimatedSprite.h"
#include "SDL.h"
#include "sprite.h"
#include "Input.h"

class Graphics;

class Title : public AnimatedSprite
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
	Sprite _selectionBox;

	int selectX = 104;
	int selectY = 250;
	int menuChoice = 0;
};

