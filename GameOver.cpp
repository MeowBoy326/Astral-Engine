#include "GameOver.h"
#include <iostream>
#include <algorithm>
#include "Input.h"

namespace {
	const int FPS = 50;
	// remove *5
	const int MAX_FRAME_TIME = 5 * 1000 / FPS; //Max amount of time a frame is allowed to last
}


GameOver::GameOver()
{
}

GameOver::GameOver(Graphics & graphics) :
//graphics, filePath, source x, source y on sprite sheet, width , height of sprite, x, y pos to start player out at (destinationRect), and timetoUpdate 100
AnimatedSprite(graphics, "endGame.png", 0, 0, 640, 480, 0, 0, 340)
{

	graphics.loadImage("endGame.png"); //loads sprite sheet in

	this->setupAnimations();
	this->playAnimation("NG0");

	this->_defeat = Sprite(graphics, "title.png", 2, 0, 241, 68, 300, 100);
	//x, y, width, height, screen pos x, screen pos y
	std::cout << "sprite added" << std::endl;
	graphics.loadImage("title.png"); //loads sprite sheet in

	this->_endGame = Sprite(graphics, "startGame.png", 0, 7, 213, 51, 210, 400);
	graphics.loadImage("startGame.png");
}


GameOver::~GameOver()
{
}

void GameOver::playNext(int num)
{
}

void GameOver::animationDone(std::string currentAnimation)
{
}

void GameOver::setupAnimations()
{
	this->addAnimation(7, 0, 0, "NG0", 640, 480, Vector2(0, 0));
	this->addAnimation(1, 1, 483, "NG1", 640, 480, Vector2(0, 0)); // we are 16 pixels down now on sprite sheet
	this->addAnimation(1, 643, 1, "NG2", 640, 480, Vector2(0, 0)); //# of frames, x, y, name(RunLeft), height, width, offset (no so empty vector)
	this->addAnimation(1, 643, 483, "NG3", 640, 480, Vector2(0, 0));
	this->addAnimation(1, 1, 965, "NG4", 640, 480, Vector2(0, 0));
}

void GameOver::update(float elapsedTime)
{
	AnimatedSprite::update(elapsedTime);
}

void GameOver::draw(Graphics & graphics)
{
	AnimatedSprite::drawTitle(graphics, this->_x, this->_y);
	//AnimatedSprite::drawTitle(graphics, this->_x - 80, this->_y + 64);
	this->_defeat.drawTitle(graphics, 180, 10);
	this->_endGame.drawTitle(graphics, 180, 425);
}
