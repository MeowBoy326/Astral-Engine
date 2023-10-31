#include "../headers/Skills.h"
#include "../headers/Graphics.h"

#include <iostream>

Skills::Skills() {}

Skills::Skills(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate) {
}

Skills::Skills(Graphics & graphics) {
}

void Skills::update(int elapsedTime, Player &player) {
	AnimatedSprite::update(elapsedTime);
}

void Skills::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Skills::drawSkills(Graphics & graphics, float x, float y) {
	AnimatedSprite::drawSkillIcon(graphics, x, y);
}

Skills::~Skills() { this->destroySprite(); }


LifeSteal::LifeSteal() {
}

LifeSteal::LifeSteal(Graphics & graphics, Vector2 spawnPoint) :
	Skills(graphics, "data\\maps\\NpcSym.png", 288, 1, 14, 15, spawnPoint, 140),
	lifeSteal(0.002f) {
	graphics.loadImage("data\\maps\\NpcSym.png"); // Loads sprite sheet in
	// This->addToInventory();
	this->setupAnimations();
	this->playAnimation("Blink");

	_skillID = ID;

}

LifeSteal::~LifeSteal() {
	this->destroySprite();
}

void LifeSteal::update(int elapsedTime, Player &player) {
	this->playAnimation("Blink");
	this->updateSkillStats(player);
	Skills::update(elapsedTime, player);
}

void LifeSteal::draw(Graphics &graphics) {
	Skills::draw(graphics);
}

void LifeSteal::drawSkills(Graphics & graphics, float x, float y) {
	AnimatedSprite::drawSkillIcon(graphics, x, y);
}

void LifeSteal::animationDone(std::string currentAnimation) {
}

void LifeSteal::setupAnimations() {
	this->addAnimation(2, 2, 84, "Blink", 16, 16, Vector2(0, 0));
}

