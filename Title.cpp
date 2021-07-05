#include "TextManager.h"
#include "Title.h"
#include <iostream>
#include <algorithm>
#include "Input.h"
#include <stdlib.h>


namespace {
	const int FPS = 50;
	// remove *5
	const int MAX_FRAME_TIME = 5 * 1000 / FPS; //Max amount of time a frame is allowed to last
}

Title::Title()
{

}

Title::Title(Graphics &graphics, Input &input, SDL_Event &event) :
//graphics, filePath, source x, source y on sprite sheet, width , height of sprite, x, y pos to start player out at (destinationRect), and timetoUpdate 100
AnimatedSprite(graphics, "dark_clouds.png", 0, 0, 640, 480, 0, 0, 140)
{
	graphics.loadImage("dark_clouds.png"); //loads sprite sheet in

	this->setupAnimations();
	this->playAnimation("zero");

	this->_title = Sprite(graphics, "title.png", 2, 0, 241, 68, 300, 100);
	//x, y, width, height, screen pos x, screen pos y
	std::cout << "sprite added" << std::endl;
	graphics.loadImage("title.png"); //loads sprite sheet in

	this->_startGame = Sprite(graphics, "startGame.png", 0, 84, 213, 40, 110, 270);
	graphics.loadImage("startGame.png");

	this->_loadGame = Sprite(graphics, "startGame.png", 0, 148, 213, 40, 340, 270);


	graphics.loadImage("npcTextBox.png");
	this->_selectionBox = Sprite(graphics, "npcTextBox.png", 0, 147, 46, 18, 140, 250);
}

Title::~Title()
{

}

bool Title::Start(Graphics &graphics, Input &input, SDL_Event &event)
{

	bool menuLoop = true;
	int LAST_UPDATE_TIME = SDL_GetTicks();

	while (menuLoop == true) {
		input.beginNewFrame(); //resets our released key pressed keys first time around doesnt matter already set to false but good to reset anyway

		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;

		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					input.keyDownEvent(event); //if we are holding key start keydown event
				}
			}
			else if (event.type == SDL_KEYUP) { // if key was released
				input.keyUpEvent(event);
			}
			else if (event.type == SDL_QUIT) {
				exit(0);
			}
		}
		if (input.wasKeyPressed(SDL_SCANCODE_RETURN) == true) {
			menuLoop = false;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_RIGHT) == true) {
			this->selectX = this->_loadGame.getX() - 6;
			menuChoice = 1;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_LEFT) == true) {
			this->selectX = this->_startGame.getX() - 6;
			menuChoice = 0;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true) {
			exit(0);
		}

		this->Title::update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURRENT_TIME_MS; //loop will go again and current time - new last update will tell us how long next frame will take
		this->Title::draw(graphics);
	}
	return false;
}

int Title::getMenuChoice() {
	std::cout << "Menu Choice = " << this->menuChoice << std::endl;
	return this->menuChoice;
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

void Title::animationDone(std::string currentAnimation) {}

void Title::setupAnimations() {
	this->addAnimation(4, 0, 0, "zero", 640, 480, Vector2(0, 0));
	this->addAnimation(1, 1, 483, "one", 640, 480, Vector2(0, 0)); // we are 16 pixels down now on sprite sheet
	this->addAnimation(1, 643, 1, "two", 640, 480, Vector2(0, 0)); //# of frames, x, y, name(RunLeft), height, width, offset (no so empty vector)
	this->addAnimation(1, 643, 483, "three", 640, 480, Vector2(0, 0));
	this->addAnimation(1, 1, 965, "four", 640, 480, Vector2(0, 0));
}

void Title::update(float elapsedTime) {
	AnimatedSprite::update(elapsedTime);
}

void Title::draw(Graphics &graphics) {
	graphics.clear(); //clear any drawings MUST do

	AnimatedSprite::drawTitle(graphics, this->_x, this->_y);
	this->_title.drawTitle(graphics, 210, 50);
	this->_startGame.drawTitle(graphics, 110, 270);
	this->_loadGame.drawTitle(graphics, 340, 270);
	this->_selectionBox.drawSelectionBox(graphics, selectX, selectY);
	this->_txt->drawVersion(graphics, 395, 460);
	this->_txt->drawDeveloper(graphics, 0, 460);
	graphics.flip();
}