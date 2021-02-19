#pragma once

#include "Graphics.h"
#include "AnimatedSprite.h"
#include "SDL.h"
#include "sprite.h"

class Graphics;

class Title : public AnimatedSprite
{
public:
	Title();
	Title(Graphics &graphics);
	~Title();
	void playNext(int num);
	void Start();
	void animationDone(std::string currentAnimation);
	void setupAnimations();
	void update(float elapsedTime);
	void draw(Graphics & graphics);
private:
	Graphics _graphics; //store graphics object
	Sprite _title;
	Sprite _startGame;
};

