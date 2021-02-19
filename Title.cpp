#include "Title.h"
#include <iostream>
#include <algorithm>
#include "Input.h"

using namespace std;

namespace {
	const int FPS = 50;
	// remove *5
	const int MAX_FRAME_TIME = 5 * 1000 / FPS; //Max amount of time a frame is allowed to last
}

Title::Title()
{

}

Title::Title(Graphics &graphics) :
//graphics, filePath, source x, source y on sprite sheet, width , height of sprite, x, y pos to start player out at (destinationRect), and timetoUpdate 100
AnimatedSprite(graphics, "dark_clouds.png", 0, 0, 640, 480, 0, 0, 140)
{

	graphics.loadImage("dark_clouds.png"); //loads sprite sheet in

	this->setupAnimations();
	this->playAnimation("zero");

	this->_title = Sprite(graphics, "title.png", 2, 0, 241, 68, 300, 100);
	//x, y, width, height, screen pos x, screen pos y
	cout << "sprite added" << endl;
	graphics.loadImage("title.png"); //loads sprite sheet in

	this->_startGame = Sprite(graphics, "startGame.png", 0, 7, 213, 51, 210, 400);
	graphics.loadImage("startGame.png");

}


Title::~Title()
{
}

void Title::playNext(int num) {
	switch (num) {
	case 0:
		this->playAnimation("zero");
		break;
	case 1:
		this->playAnimation("one");
		break;
	case 2:
		this->playAnimation("two");
		break;
	case 3:
		this->playAnimation("three");
		break;
	case 4:
		this->playAnimation("four");
		break;
	}
}

void Title::Start()
{

}

void Title::animationDone(std::string currentAnimation) {}

void Title::setupAnimations() {
	this->addAnimation(4, 0, 0, "zero", 640, 480, Vector2(0, 0));
	this->addAnimation(1, 1, 483, "one", 640, 480, Vector2(0, 0)); // we are 16 pixels down now on sprite sheet
	this->addAnimation(1, 643, 1, "two", 640, 480, Vector2(0, 0)); //# of frames, x, y, name(RunLeft), height, width, offset (no so empty vector)
	this->addAnimation(1, 643, 483, "three", 640, 480, Vector2(0, 0));
	this->addAnimation(1, 1, 965, "four", 640, 480, Vector2(0, 0));
	/*
	this->addAnimation(1, 3, 16, "IdleRightUp", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 3, 0, "RunLeftUp", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 3, 16, "RunRightUp", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 6, 0, "LookDownLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 6, 16, "LookDownRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 7, 0, "LookBackwardsLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 7, 16, "LookBackwardsRight", 16, 16, Vector2(0, 0));*/
}

void Title::update(float elapsedTime) {
	AnimatedSprite::update(elapsedTime);
}

void Title::draw(Graphics &graphics) {
	AnimatedSprite::drawTitle(graphics, this->_x, this->_y);
	this->_title.drawTitle(graphics, 210, 50);
	this->_startGame.drawTitle(graphics, 210, 360);
}