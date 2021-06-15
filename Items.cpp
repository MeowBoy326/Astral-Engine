#include "Items.h"

#include "Graphics.h"
#include <iostream>

namespace itemConstants {
	Inventory::InventoryItem* hPot = new Inventory::InventoryItem;
}

Items::Items()
{
}

Items::Items(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate) {}

Items::Items(Graphics & graphics)
{
}

void Items::update(int elapsedTime, Player &player) {
	AnimatedSprite::update(elapsedTime);
}

void Items::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}



void Items::addToInventory(int type) {
	if (type == 0) {
		itemConstants::hPot->type = HEALTH_POT;
		this->addItem(itemConstants::hPot);
		std::cout << "Items::addTo success!" << std::endl;
	}

	if (itemConstants::hPot->type != NULL) {
		std::cout << "null..." << std::endl;
	}
}

Items::~Items()
{
}

/*
*	Health Potion Item
*/

HealthPotion::HealthPotion()
{
}

HealthPotion::HealthPotion(Graphics & graphics, Vector2 spawnPoint) :
	Items(graphics, "NpcSym.png", 288, 1, 14, 15, spawnPoint, 140),
	hpGain(3)
{
	graphics.loadImage("NpcSym.png"); //loads sprite sheet in
	//this->addToInventory();
	this->setupAnimations();
	this->playAnimation("Blink");
}

void HealthPotion::addToInventory() {
}

void HealthPotion::update(int elapsedTime, Player &player) {
	this->playAnimation("Blink");
	Items::update(elapsedTime, player);
}

void HealthPotion::draw(Graphics &graphics) {
	Items::draw(graphics);
}

void HealthPotion::animationDone(std::string currentAnimation)
{
}

void HealthPotion::setupAnimations()
{
	this->addAnimation(2, 2, 84, "Blink", 16, 16, Vector2(0, 0));
}

/*
*	Permenant HP Item
*/
PermHP::PermHP() {};

PermHP::PermHP(Graphics &graphics, Vector2 spawnPoint) :
	Items(graphics, "NpcSym.png", 288, 1, 14, 15, spawnPoint, 140),
	maxHPgain(5)
{
	graphics.loadImage("NpcSym.png"); //loads sprite sheet in
	//this->addToInventory();
	this->setupAnimations();
	this->playAnimation("Blink");
}

void PermHP::update(int elapsedTime, Player & player) {
	this->playAnimation("Blink");
	Items::update(elapsedTime, player);
}

void PermHP::draw(Graphics & graphics) {
	Items::draw(graphics);
}

void PermHP::animationDone(std::string currentAnimation) {
}

void PermHP::setupAnimations() {
	this->addAnimation(2, 2, 101, "Blink", 16, 16, Vector2(0, 0));
}

/*
*	Key Item
*/

Key::Key() {};

Key::Key(Graphics &graphics, Vector2 spawnPoint) :
	Items(graphics, "NpcSym.png", 288, 1, 14, 15, spawnPoint, 140)
{
	graphics.loadImage("NpcSym.png"); //loads sprite sheet in
	//this->addToInventory();
	this->setupAnimations();
	this->playAnimation("Blink");
}

void Key::draw(Graphics &graphics) {

}

void Key::update(int elapsedTime, Player & player) {

}

void Key::animationDone(std::string currentAnimation) {

}

void Key::setupAnimations() {

}

